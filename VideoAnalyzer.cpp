//
// Created by ppeczek on 5/12/17.
//

#include "VideoAnalyzer.h"

int VideoAnalyzer::createPattern() {
    vector<Mat> frames;

    if (stat(framesPath.c_str(), &info) != 0) {
        mkdir(framesPath.c_str(), ACCESSPERMS);
    }

    VideoCapture cap;
    cap.open(videoPath);

    if (!cap.isOpened()) {
        cout << "nima video" << endl;
        return -1;
    }

    for (int j=0; j<nFrames; ++j) {
        String framePath = framesPath + "frame_" + to_string(j+1) + ".png";
        cap.set(CAP_PROP_POS_FRAMES, j*200);
        Mat frame;
        cap >> frame;

        if(frame.empty()) {
            break;
        }

        frames.push_back(frame);
        imwrite(framePath, frame);
    }

    Mat pattern = VideoAnalyzer::medianMerge(frames);
    imwrite(patternPath, pattern);
}

Mat VideoAnalyzer::medianMerge(vector<Mat> frames) {
    Mat result;
    bool firstFrame = true;

    int channels = 0;
    int nRows = 0;
    int nCols = 0;

    for (auto&& frame : frames) {
        if (firstFrame) {
            // copy first frame to result
            result = frame;

            // save image parameters
            channels = frame.channels();
            nRows = frame.rows;
            nCols = frame.cols;

            firstFrame = false;
        }
        else {
            assert(frame.channels() == channels);
            assert(frame.rows == nRows);
            assert(frame.cols == nCols);
        }
        cvtColor(frame, frame, CV_BGR2HSV);
    }

    vector<int> h(nFrames);
    vector<int> s(nFrames);
    vector<int> v(nFrames);
    vector<long> window(nFrames);

    switch (channels) {
        case 1:
            break;
        case 3:
            for (int y=0; y< nRows; ++y) {
                for (int x=0; x< nCols; ++x) {
                    int counter = 0;
                    for (auto&& frame : frames) {
                        h[counter] = frame.at<Vec3b>(y, x)[0];
                        s[counter] = frame.at<Vec3b>(y, x)[1];
                        v[counter] = frame.at<Vec3b>(y, x)[2];
                        ++counter;
                    }
                    sort(h.begin(), h.end());
                    sort(s.begin(), s.end());
                    sort(v.begin(), v.end());
                    result.at<Vec3b>(y, x)[0] = h[nFrames/2];
                    result.at<Vec3b>(y, x)[1] = s[nFrames/2];
                    result.at<Vec3b>(y, x)[2] = v[nFrames/2];
                }
            }
            break;
        default:
            throw;
    }

    cvtColor(result, result, CV_HSV2BGR);
    return result;
}

void VideoAnalyzer::analyze(bool saveToFile, bool showImage) {
    VideoCapture cap;
    cap.open(videoPath);

    Mat frame;
    Mat pattern = imread(patternPath);
    Mat result;

    Moments mu;
    Point2f mc;

    int resultX, resultY;
    int oldX, oldY;
    bool firstFrame = true;

    std::stringstream buf;

    for (;;) {
        cap >> frame;

        if(frame.empty())
            break;

        if (!inverted) {
            result = pattern - frame;
        }
        else {
            result = frame - pattern;
        }

        cvtColor(result, result, CV_BGR2GRAY);
        resize(result, result, Size(), 0.25, 0.25, INTER_CUBIC);
        resize(result, result, Size(), 4, 4, INTER_CUBIC);
        threshold(result, result, 50, 255, 0);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(result, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        Mat drawing = Mat::zeros(result.size(), CV_8UC3);

        for (vector<vector<Point>>::iterator it = contours.begin(); it != contours.end(), contours.size() == 0; ++it) {
            mu = moments(contours[0], false);
            mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);

            if (mc.x > maxX || mc.x < minX || mc.y > maxY || mc.y < minY) {
                contours.erase(it);
            }
        }

        resultX = -1;
        resultY = -1;
        if (contours.size() > 0) {
            sort(contours.begin(), contours.end(), more_vectors);
            mu = moments(contours[0], false);
            mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
            if (!firstFrame) {
                while(distance(Point(oldX, oldY), mc) > 100000 && !contours.empty()) {
                    contours.erase(contours.begin());
                    mu = moments(contours[0], false);
                    mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
                }
                if (contours.size()) {
                    resultX = mc.x;
                    resultY = mc.y;
                }
            }
            else {
                resultX = mc.x;
                resultY = mc.y;
                firstFrame = false;
            }
            oldX = resultX;
            oldY = resultY;
        }

        buf << resultX << " " << resultY << endl;

        if (showImage) {
//            circle(drawing, mc, 1, color);
            drawContours(drawing, contours, 0, color);

            namedWindow("Contours", CV_WINDOW_AUTOSIZE);
            imshow("Contours", drawing);
//            imshow("Contours", result);

            int c = waitKey(30);
            if(c == 'q' || c == 'Q' || (c & 255) == 27) {
                break;
            }
        }

    }

    if (saveToFile) {
        ofstream resultFile;
        resultFile.open(localizationsPath);
        resultFile << buf.str();
        resultFile.close();
    }

}

void VideoAnalyzer::drawPath() {
    Mat pattern = imread(patternPath);
    Mat pointsImage = Mat::zeros(pattern.size(), CV_8UC3);
    Mat pathsImage = Mat::zeros(pattern.size(), CV_8UC3);

    for (int y=0; y<pattern.rows; ++y) {
        for (int x=0; x<pattern.cols; ++x) {
            pointsImage.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
            pathsImage.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
        }
    }

    std::fstream infile(localizationsPath);
    float x, y;
    float oldX, oldY;
    bool firstPoint = true;

    while (infile >> x >> y) {
        if (x != -1 && y != -1) {
            circle(pointsImage, Point(x, y), 0, color);
            circle(pathsImage, Point(x, y), 0, color);
            if (!firstPoint) {
                line(pathsImage, Point(x, y), Point(oldX, oldY), color);
            }
            oldX = x;
            oldY = y;
            firstPoint = false;
        }
    }

    imwrite(pointsImagePath, pointsImage);
    imwrite(pathsImagePath, pathsImage);
}