//
// Created by ppeczek on 5/12/17.
//

#ifndef MEDIANFILTER_VIDEOANALYZER_H
#define MEDIANFILTER_VIDEOANALYZER_H

#include <iostream>
#include <fstream>

#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>

#include "Settings.h"
#include "Commons.h"
#include "AnalyzerSettings.h"

using namespace cv;
using namespace std;

struct stat info;

bool more_vectors(const vector<Point>& a, const vector<Point>& b) {
    return a.size() > b.size();
}

double distance(Point a, Point b) {
    return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}


class VideoAnalyzer {
public:
    VideoAnalyzer(AnalyzerSettings s, Point a, Point b, Point c, Point d) :
            settings(s), cornerA(a), cornerB(b), cornerC(c), cornerD(d) {
        // params
        platformHeight = maxY - minY;
        platformWidth = maxX - minX;

        thresholdFactor = 20;
        blurFactor = 5;
    };
    ~VideoAnalyzer() {};

    Mat createPattern();
    void analyze(bool debug = false, bool applyKalman = true);
    void myBlur(Mat& src, Mat& dst);
    void applyKalmanFilter();

    void drawPath(String, String, String, bool showBoundaries = false);

    void drawMeasuredPath(bool showBoundaries = false);
    void drawMeasuredMinPath(bool showBoundaries = false);
    void drawEstimatedPath(bool showBoundaries = false);

    Point relativeLocalization(Point p) {
        return Point(p.x - minX, p.y - minY);
    };

    void drawBoundaries(Mat image, Scalar color, int lineWidth) {
        line(image, Point(cornerA), Point(cornerB), color, lineWidth);
        line(image, Point(cornerB), Point(cornerC), color, lineWidth);
        line(image, Point(cornerC), Point(cornerD), color, lineWidth);
        line(image, Point(cornerD), Point(cornerA), color, lineWidth);
    };

private:
    AnalyzerSettings settings;

    // video attrs
    int minX;
    int maxX;
    int minY;
    int maxY;

    Point cornerA;
    Point cornerB;
    Point cornerC;
    Point cornerD;

    int platformHeight;
    int platformWidth;

    int thresholdFactor;
    int blurFactor;

    // Kalman Filter
    KalmanFilter KF;
    Mat_<float> measurement;
    Mat estimated;

};

#endif //MEDIANFILTER_VIDEOANALYZER_H
