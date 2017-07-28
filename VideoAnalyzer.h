//
// Created by ppeczek on 5/12/17.
//

#ifndef WUSHUVIDEOANALYZER_VIDEOANALYZER_H
#define WUSHUVIDEOANALYZER_VIDEOANALYZER_H

#include <iostream>
#include <fstream>
#include <iomanip>

#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>

#include "Settings.h"
#include "Commons.h"
#include "AnalyzerSettings.h"
#include "CandidateContours.h"
#include "ResultPoint.h"

using namespace cv;
using namespace std;


class VideoAnalyzer {
public:
    VideoAnalyzer(AnalyzerSettings s, vector<Point> cs) : settings(s), corners(cs) {};
    ~VideoAnalyzer() {};

    Mat createPattern();
    void analyze();
    void myBlur(Mat& src, Mat& dst);

    void drawPath(String, String, Scalar, bool showBoundaries = false);

    void drawMeasuredPath(bool showBoundaries = false);
    void drawKalmanPath(bool showBoundaries = false);

    struct stat info;

private:
    AnalyzerSettings settings;

    // video attrs
    vector<Point> corners;
    double cornerDiagonal;

    // Kalman Filter
    KalmanFilter KF;
    Mat_<float> measurement;
    Mat estimated;

    void drawBoundaries(Mat image, Scalar color, int lineWidth) {
        int max = corners.size();
        for (int i=0; i<max; ++i) {
            int nextIndex = (i+1) % max;
            line(image, Point(corners[i]), Point(corners[nextIndex]), color, lineWidth);
        }
    };

    string formatDouble(double val) {
        stringstream stream;
        stream << fixed << setprecision(0) << val;
        return stream.str();
    };

};

#endif //WUSHUVIDEOANALYZER_VIDEOANALYZER_H
