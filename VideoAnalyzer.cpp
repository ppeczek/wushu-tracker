//
// Created by ppeczek on 5/12/17.
//

#include "VideoAnalyzer.h"

Mat VideoAnalyzer::createPattern() {
    vector<Mat> frames;

//    cout << settings.videoPath << endl;
    VideoCapture cap;
    cap.open(settings.videoPath);

    if (!cap.isOpened()) {
        cout << "nima video" << endl;
        return Mat();
    }

    for (int j=0; j<settings.nFrames; ++j) {
        String framePath = settings.framesPath + "frame_" + to_string(j+1) + ".png";
        cap.set(CAP_PROP_POS_FRAMES, j*200);
        Mat frame;
        cap >> frame;

        if(frame.empty()) {
            break;
        }

        frames.push_back(frame);
        imwrite(framePath, frame);
    }

    Mat pattern = Commons::medianMerge(frames);
    imwrite(settings.patternPath, pattern);

    return pattern;
}

void VideoAnalyzer::analyze() {
    VideoCapture cap;
    cap.open(settings.videoPath);

    Mat pattern = imread(settings.patternPath);
    Mat frame;
    Mat result;

    Moments mu;
    Point2f mc;
    CandidateContours athleteContours;

    int resultX, resultY;
    int oldX = -1, oldY = -1;
    bool firstFrame = true;
    int cntr = 0;

    std::stringstream buf, bufKalman;
    vector<ResultPoint> resultPoints, resultKalmanPoints;

    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.3;
    int thickness = 1;
    int baseline = 0;

    if (pattern.empty()) {
        cout << "create pattern";
        pattern = createPattern();
    }

    settings.adjustScaling(pattern);
    if (settings.doResize) {
        for (auto&& corner: corners) {
            corner *= settings.resizeFactor;
        }
    }

    if (settings.doResize) {
        resize(pattern, pattern, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
    }

    if (stat(settings.resultsPath.c_str(), &info) != 0) {
        mkdir(settings.resultsPath.c_str(), ACCESSPERMS);
    }

    // Kalman Filter initialization
    if (settings.applyKalman) {
        KF = KalmanFilter(4, 2, 0);
        KF.transitionMatrix = (Mat_<float>(4, 4) << 1,0,1,0,  0,1,0,1,  0,0,1,0,  0,0,0,1);
        measurement = Mat_<float>(2, 1);
        measurement.setTo(Scalar(0));

        setIdentity(KF.measurementMatrix);
        setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-3));
        setIdentity(KF.errorCovPost, Scalar::all(.1));
    }

    while (true) {
        cap >> frame;

        if (frame.empty())
            break;

        if (settings.doResize) {
            resize(frame, frame, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
        }

        // abs diff so it doesn't matter whether background is lighter or darker
        absdiff(pattern, frame, result);

        // blur
        myBlur(result, result);

        // threshold
        cvtColor(result, result, CV_BGR2GRAY);
        threshold(result, result, settings.thresholdFactor, 255, 0);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;

        findContours(result, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        vector<CandidateContours> candidate_contours;
        for(auto&& contour: contours) {
            if (firstFrame) {
                candidate_contours.emplace_back(CandidateContours(contour, corners, cornerDiagonal, Point(-1, -1)));
                firstFrame = false;
            }
            else {
                candidate_contours.emplace_back(CandidateContours(contour, corners, cornerDiagonal, mc));
            }
        }

        resultX = -1;
        resultY = -1;
        if (candidate_contours.size()) {
            sort(candidate_contours.begin(), candidate_contours.end());
            athleteContours = candidate_contours[0];
            mu = athleteContours.getMu();
            mc = athleteContours.getMc();
            resultX = mc.x;
            resultY = mc.y;
            measurement(0) = mc.x;
            measurement(1) = mc.y;
        }

        if (resultX != -1 && resultY != -1) {
            ResultPoint rp(Point(resultX, resultY), cntr);
            resultPoints.emplace_back(rp);
            buf << rp << endl;
        }

        // Kalman Filter magic
        if (settings.applyKalman) {
            // predict phase
            Mat prediction = KF.predict();

            if (resultX != -1 && resultY != -1) {
                // correct phase
                estimated = KF.correct(measurement);
            } else {
                estimated = prediction;
            }
            ResultPoint rp(Point(estimated.at<float>(0), estimated.at<float>(1)), cntr);
            resultKalmanPoints.emplace_back(rp);
            bufKalman << rp << endl;
        }

        ++cntr;

        if (settings.debug) {
            Mat drawing = Mat(frame);
            cvtColor(result, result, CV_GRAY2BGR);
//            absdiff(drawing, result, drawing);

            circle(drawing, mc, 3, Settings::redColor, -1);
            circle(result, mc, 3, Settings::redColor, -1);
            if (settings.applyKalman) {
                circle(drawing, Point2f(estimated.at<float>(0), estimated.at<float>(1)), 3, Settings::greenColor, -1);
                circle(result, Point2f(estimated.at<float>(0), estimated.at<float>(1)), 3, Settings::greenColor, -1);
            }
            for (int i=0; i<contours.size(); ++i) {
                Scalar contourColor = Settings::whiteColor;
                if (contours[i] == athleteContours.getContour()) {
                    contourColor = Settings::redColor;
                }
                drawContours(drawing, contours, i, contourColor);
            }
            drawBoundaries(drawing, Settings::blueColor, 2);

            if (settings.debug && settings.showDetailText) {
                for (auto&& candidate: candidate_contours) {
                    Scalar contourColor = Settings::whiteColor;
                    if (candidate.getContour() == athleteContours.getContour()) {
                        contourColor = Settings::redColor;
                    }
                    string contourText = "";
                    contourText = "c: " + formatDouble(candidate.getContourSize()) + ", " +
                                  //                        "cd: " + formatDouble(candidate.getCornerDistances()) + ", " +
                                  "cd: [" + to_string(candidate.getVectorCenter().x) + ", " + to_string(candidate.getVectorCenter().y) + "], " +
                                  "lp: " + formatDouble(candidate.getLastPositionDistance());
                    Point textOrg(candidate.getMc().x + 10, candidate.getMc().y + 10);
                    putText(drawing, contourText, textOrg, fontFace, fontScale, contourColor, thickness, 8);
                }
            }

//            drawing = Commons::fourPointsTransformation(drawing, corners);

            namedWindow("Wushu Video Analyzer", CV_WINDOW_AUTOSIZE);
            imshow("Wushu Video Analyzer", drawing);
//            imshow("Wushu Video Analyzer", result);

            int c = waitKey(30);
            if(c == 'q' || c == 'Q' || (c & 255) == 27) {
                break;
            }
        }

    }

    ofstream resultFile;
    resultFile.open(settings.localizationsPath);
    resultFile << buf.str();
    resultFile.close();

    if (settings.applyKalman) {
        ofstream resultFile2;
        resultFile2.open(settings.localizationsKalmanPath);
        resultFile2 << bufKalman.str();
        resultFile2.close();
    }

}

void VideoAnalyzer::myBlur(Mat& src, Mat& dst) {
    resize(src, dst, Size(), 0.1, 0.1, INTER_CUBIC);
    resize(src, dst, Size(), 10, 10, INTER_CUBIC);
//        medianBlur(src, dst, settings.blurFactor);
//        GaussianBlur(src, dst, Size(settings.blurFactor, settings.blurFactor)); TODO: FIXME
//        blur(src, dst, settings.blurFactor);
}

void VideoAnalyzer::drawPath(String inputPath, String outputPathLines, Scalar pathColor, bool showBoundaries) {
    Mat pattern = imread(settings.patternPath);
    Mat pathsImage = Mat::zeros(pattern.size(), CV_8UC3);
    int timestamp;

    for (int y=0; y<pattern.rows; ++y) {
        for (int x=0; x<pattern.cols; ++x) {
            pathsImage.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
        }
    }

    std::fstream infile(inputPath);
    float x, y;
    float oldX, oldY;
    bool firstPoint = true;

    if (showBoundaries) {
        drawBoundaries(pathsImage, Settings::blueColor, 2);
    }

    while (infile >> x >> y >> timestamp) {
        if (x != -1 && y != -1) {
            circle(pathsImage, Point(x, y), 0, pathColor);
            if (!firstPoint) {
                line(pathsImage, Point(x, y), Point(oldX, oldY), pathColor, 2);
            }
            oldX = x;
            oldY = y;
            firstPoint = false;
        }
    }

//    pathsImage = Commons::fourPointsTransformation(pathsImage, corners);

    imwrite(outputPathLines, pathsImage);
}

void VideoAnalyzer::drawMeasuredPath(bool showBoundaries) {
    drawPath(settings.localizationsPath, settings.pathsImagePath, Settings::redColor, showBoundaries);
}

void VideoAnalyzer::drawKalmanPath(bool showBoundaries) {
    drawPath(settings.localizationsKalmanPath, settings.pathsImageKalmanPath, Settings::greenColor, showBoundaries);
}
