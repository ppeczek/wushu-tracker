//
// Created by ppeczek on 7/20/17.
//

#include "Commons.h"


const int Commons::fontFace = FONT_HERSHEY_SIMPLEX;
const double Commons::fontScale = 0.3;
const int Commons::thickness = 1;
const int Commons::baseline = 0;

const Point Commons::nullPoint = Point(-1, -1);

Mat Commons::medianMerge(vector<Mat> frames) {
    Mat result;
    bool firstFrame = true;

    int channels = 0;
    int nRows = 0;
    int nCols = 0;
    int nFrames = frames.size();

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

void Commons::resetPoint(Point &point) {
    point = Commons::nullPoint;
}

String Commons::formatDouble(double val) {
    stringstream stream;
    stream << fixed << setprecision(0) << val;
    return stream.str();
}

bool Commons::isTransformatedPointValid(const Point& point) {
    return point.x > 0 && point.x < Settings::outputImageWidth && point.y > 0 && point.y < Settings::outputImageHeight;
}