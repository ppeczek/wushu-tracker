//
// Created by ppeczek on 5/12/17.
//

#ifndef MEDIANFILTER_VIDEOANALYZER_H
#define MEDIANFILTER_VIDEOANALYZER_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"

#include <iostream>
#include <fstream>

#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>

using namespace cv;
using namespace std;

struct stat info;
const int nFrames = 10;
const String mediaPath = "media/";
const String videoName = "video.mp4";
const String patternName = "pattern.png";
const String localizationsFilename = "result.dat";
const String pointsImageFilename = "points.png";
const String pathsImageFilename = "paths.png";
const int minX = 0;
const int maxX = 1260;
const int minY = 465;
const int maxY = 750;
Scalar color = Scalar(0, 0, 255);

bool more_vectors(const vector<Point>& a, const vector<Point>& b) {
    return a.size() > b.size();
}

double distance(Point a, Point b) {
    return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}


class VideoAnalyzer {

private:
    String dir;
    String samplePath;
    String videoPath;
    String patternPath;
    String framesPath;
    String localizationsPath;
    String pointsImagePath;
    String pathsImagePath;

    Point cornerA;
    Point cornerB;
    Point cornerC;
    Point cornerD;

    bool inverted;

public:
    VideoAnalyzer(String _dir, Point a, Point b, Point c, Point d, bool inverted = false) :
            dir(_dir), cornerA(a), cornerB(b), cornerC(c), cornerD(d), inverted(inverted) {
        samplePath = mediaPath + dir + "/";
        videoPath = samplePath + videoName;
        patternPath = samplePath + patternName;
        framesPath = samplePath + "frames/";
        localizationsPath = samplePath + localizationsFilename;
        pointsImagePath = samplePath + pointsImageFilename;
        pathsImagePath = samplePath + pathsImageFilename;
    }
    ~VideoAnalyzer() {};

    int createPattern();
    static Mat medianMerge(vector<Mat> frames);
    void analyze(bool saveToFile = true, bool showImage = false);
    void drawPath();
};


#endif //MEDIANFILTER_VIDEOANALYZER_H