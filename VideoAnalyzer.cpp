//
// Created by ppeczek on 5/12/17.
//

#include "VideoAnalyzer.h"

Mat VideoAnalyzer::createPattern() {
    vector<Mat> frames;

    VideoCapture cap;
    cap.open(settings.videoPath);

    if (stat(settings.framesPath.c_str(), &info) != 0) {
        mkdir(settings.framesPath.c_str(), ACCESSPERMS);
    }

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

void VideoAnalyzer::analyze(bool debug, bool applyKalman) {
    VideoCapture cap;
    cap.open(settings.videoPath);

    Mat pattern = imread(settings.patternPath);
    Mat frame;
    Mat result;

    Moments mu;
    Point2f mc;

    int resultX, resultY;
    int oldX, oldY;
    int savedX = -1, savedY = -1;
    bool firstFrame = true;
    int cntr = 0;

    std::stringstream buf, buf2;

    if (pattern.empty()) {
        cout << "create pattern";
        pattern = createPattern();
    }

    if (stat(settings.resultsPath.c_str(), &info) != 0) {
        mkdir(settings.resultsPath.c_str(), ACCESSPERMS);
    }

    if (applyKalman) {
        KF = KalmanFilter(4, 2, 0);
        KF.transitionMatrix = (Mat_<float>(4, 4) << 1,0,1,0,  0,1,0,1,  0,0,1,0,  0,0,0,1);
        measurement = Mat_<float>(2, 1);
        measurement.setTo(Scalar(0));

//    infile >> KF.statePre.at<float>(0) >> KF.statePre.at<float>(1) >> timestamp;
//    KF.statePre.at<float>(2) = 0;
//    KF.statePre.at<float>(3) = 0;
        setIdentity(KF.measurementMatrix);
        setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-3));
        setIdentity(KF.errorCovPost, Scalar::all(.1));
    }

    while (true) {
        cap >> frame;

        if (frame.empty())
            break;

        // abs diff so it doesn't matter whether background is lighter or darker
        absdiff(pattern, frame, result);

        // blur
        myBlur(result, result);

        // threshold
        cvtColor(result, result, CV_BGR2GRAY);
        threshold(result, result, thresholdFactor, 255, 0);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;

        findContours(result, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

//        Mat drawing = Mat::zeros(result.size(), CV_8UC3);
        Mat drawing = Mat(frame);

        if (!debug) {
            for (int i = 0; i < contours.size(); ++i) {
                drawContours(drawing, contours, i, Settings::whiteColor);
            }
        }

//        for (vector<vector<Point>>::iterator it = contours.begin(); it != contours.end(), contours.size() == 0; ++it) {
//            mu = moments(contours[0], false);
//            mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
//
//            if (mc.x > maxX || mc.x < minX || mc.y > maxY || mc.y < minY) {
//                contours.erase(it);
//            }
//        }

        resultX = -1;
        resultY = -1;
        if (contours.size() > 0) {
            sort(contours.begin(), contours.end(), more_vectors);
            mu = moments(contours[0], false);
            mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
//            if (!firstFrame) {
//                while(distance(Point(oldX, oldY), mc) > 10000 && !contours.empty()) {
//                    contours.erase(contours.begin());
//                    mu = moments(contours[0], false);
//                    mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
//                }
//                if (contours.size()) {
//                    resultX = mc.x;
//                    resultY = mc.y;
//                }
//            }
//            else {
                resultX = mc.x;
                resultY = mc.y;
                measurement(0) = mc.x;
                measurement(1) = mc.y;
                firstFrame = false;
//            }
            oldX = resultX;
            oldY = resultY;
        }

        if (resultX != -1 && resultY != -1) {
            buf << resultX << " " << resultY << " " << cntr << endl;
            if (resultX != savedX && resultY != savedY) {
                savedX = resultX;
                savedY = resultY;
                buf2 << resultX << " " << resultY << " " << cntr << endl;
            }
            if (applyKalman) {
                // predict phase
                Mat prediction = KF.predict();
                Point predictPt(prediction.at<float>(0), prediction.at<float>(1));
                estimated = KF.correct(measurement);
            }
        }

        ++cntr;

        if (debug) {
            cvtColor(result, result, CV_GRAY2BGR);
//            absdiff(drawing, result, drawing);

            circle(drawing, mc, 1, Settings::redColor);
            circle(result, mc, 1, Settings::redColor);
            if (applyKalman) {
                circle(drawing, Point2f(estimated.at<float>(0), estimated.at<float>(1)), 1, Settings::greenColor);
                circle(result, Point2f(estimated.at<float>(0), estimated.at<float>(1)), 1, Settings::greenColor);
            }
            for (int i=0; i<contours.size(); ++i) {
                drawContours(drawing, contours, i, Settings::whiteColor);
            }
            drawContours(drawing, contours, 0, Settings::redColor);

            namedWindow("Wushu Video Analyzer", CV_WINDOW_AUTOSIZE);
            imshow("Wushu Video Analyzer", drawing);
//            imshow("Wushu Video Analyzer", result);

            int c = waitKey(30);
            if(c == 'q' || c == 'Q' || (c & 255) == 27) {
                break;
            }
        }

    }

//    imwrite(samplePath + "test-1.png", result);

    if (!debug) {
        ofstream resultFile;
        resultFile.open(settings.localizationsPath);
        resultFile << buf.str();
        resultFile.close();

        ofstream resultFile2;
        resultFile2.open(settings.localizationsMinPath);
        resultFile2 << buf2.str();
        resultFile2.close();
    }

}

void VideoAnalyzer::myBlur(Mat& src, Mat& dst) {
    resize(src, dst, Size(), 0.25, 0.25, INTER_CUBIC);
    resize(src, dst, Size(), 4, 4, INTER_CUBIC);
//        medianBlur(src, dst, blurFactor);
//        GaussianBlur(src, dst, Size(blurFactor, blurFactor)); TODO: FIXME
//        blur(src, dst, blurFactor);
}

void VideoAnalyzer::applyKalmanFilter() {
    /***
     * dimensionality of state
     * dimensionality of the measurement
     * dimensionality of the control vector
     */
    std::fstream infile(settings.localizationsMinPath);
    std::stringstream buf;
    int timestamp;

    KalmanFilter KF(4, 2, 0);
    KF.transitionMatrix = (Mat_<float>(4, 4) << 1,0,1,0,  0,1,0,1,  0,0,1,0,  0,0,0,1);
    Mat_<float> measurement(2, 1);
    measurement.setTo(Scalar(0));

//    KalmanFilter KF(2, 1, 0);
//    KF.transitionMatrix = (Mat_<float>(2, 2) << 1, 1, 0, 1);
//    Mat_<float> measurement(2, 1);
//    measurement.setTo(Scalar(0));

    infile >> KF.statePre.at<float>(0) >> KF.statePre.at<float>(1) >> timestamp;
    KF.statePre.at<float>(2) = 0;
    KF.statePre.at<float>(3) = 0;
    setIdentity(KF.measurementMatrix);

    setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-3));
    setIdentity(KF.errorCovPost, Scalar::all(.1));

    while (infile >> measurement(0) >> measurement(1) >> timestamp) {
        // First predict, to update the internal statePre variable
        Mat prediction = KF.predict();
        Point predictPt(prediction.at<float>(0), prediction.at<float>(1));

//        cout << measurement(0) << " " << measurement(1) << " " << timestamp << endl;
        // The "correct" phase that is going to use the predicted value and our measurement
        Mat estimated = KF.correct(measurement);
        Point statePt(estimated.at<float>(0), estimated.at<float>(1));
        buf << statePt.x << " " << statePt.y << " " << timestamp << endl;
//        cout << "Prediction:\t" << predictPt.x << " " << predictPt.y << endl;
//        cout << "Kalman:\t\t" << statePt.x << " " << statePt.y << endl;
//        cout << "Measurement:\t" << measurement(0) << " " << measurement(1) << endl;
    }

    ofstream resultFile;
    resultFile.open(settings.estimatedLocalizationsPath);
    resultFile << buf.str();
    resultFile.close();
}

void VideoAnalyzer::drawPath(String inputPath, String outputPathPoints, String outputPathLines, bool showBoundaries) {
    Mat pattern = imread(settings.patternPath);
    Mat pointsImage = Mat::zeros(pattern.size(), CV_8UC3);
    Mat pathsImage = Mat::zeros(pattern.size(), CV_8UC3);
    int timestamp;

    for (int y=0; y<pattern.rows; ++y) {
        for (int x=0; x<pattern.cols; ++x) {
            pointsImage.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
            pathsImage.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
        }
    }

    std::fstream infile(inputPath);
    float x, y;
    float oldX, oldY;
    bool firstPoint = true;

    if (showBoundaries) {
        drawBoundaries(pointsImage, Settings::blueColor, 2);
        drawBoundaries(pathsImage, Settings::blueColor, 2);
    }

    while (infile >> x >> y >> timestamp) {
        if (x != -1 && y != -1) {
            circle(pointsImage, Point(x, y), 0, Settings::redColor);
            circle(pathsImage, Point(x, y), 0, Settings::redColor);
            if (!firstPoint) {
                line(pathsImage, Point(x, y), Point(oldX, oldY), Settings::redColor, 2);
            }
            oldX = x;
            oldY = y;
            firstPoint = false;
        }
    }

    imwrite(outputPathPoints, pointsImage);
    imwrite(outputPathLines, pathsImage);
}

void VideoAnalyzer::drawMeasuredPath(bool showBoundaries) {
    drawPath(settings.localizationsPath, settings.pointsImagePath, settings.pathsImagePath, showBoundaries);
}

void VideoAnalyzer::drawMeasuredMinPath(bool showBoundaries) {
    drawPath(settings.localizationsMinPath, settings.pointsMinImagePath, settings.pathsMinImagePath, showBoundaries);
}

void VideoAnalyzer::drawEstimatedPath(bool showBoundaries) {
    drawPath(settings.estimatedLocalizationsPath, settings.estimatedPointsImagePath, settings.estimatedPathsImagePath, showBoundaries);
}

