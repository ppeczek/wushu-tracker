//
// Created by ppeczek on 5/12/17.
//

#ifndef WUSHUVIDEOANALYZER_VIDEOANALYZER_H
#define WUSHUVIDEOANALYZER_VIDEOANALYZER_H

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>

#include <opencv2/tracking/tracker.hpp>
#include <opencv2/tracking/tldDataset.hpp>

#include "Settings.h"
#include "Commons.h"

#include "AnalyzerSettings.h"
#include "Painter.h"

#include "Platform.h"
#include "ProcessedImage.h"
#include "AnalysisResultPoint.h"
#include "KalmanEstimator.h"

using namespace cv;
using namespace std;


class VideoAnalyzer {
public:
    VideoAnalyzer(AnalyzerSettings s, vector<Point> corners) : settings(s), platform(corners) {};
    VideoAnalyzer(const VideoAnalyzer& va) : settings(va.settings), platform(va.platform) {};
   ~VideoAnalyzer() {};

    enum TrackingModes {
        MIL,
        BOOSTING,
        KCF,
        TLD,
        MEDIANFLOW,
        GOTURN
    };

    struct stat info;

    // PUBLIC METHODS
    Mat createPattern();
    void analyze();
    void opencvAnalyze(TrackingModes);
    void drawPaths();

private:
    AnalyzerSettings settings;
    Platform platform;
    Mat pattern;
    ProcessedImage processedImage;

    int cntr;

    // Kalman Filter
    KalmanEstimator KE;

    // cartesian coordinates (in rectangle)
    Point transformatedCoordinates;
    Point transformatedKalmanCoordinates;

    // result containers
    vector<AnalysisResultPoint> resultPoints;
    vector<AnalysisResultPoint> resultKalmanPoints;
    std::stringstream buf;
    std::stringstream bufKalman;

    // PRIVATE METHODS

    int prepareAnalysis();
    int opencvAnalyze(Ptr<Tracker>);

};

#endif //WUSHUVIDEOANALYZER_VIDEOANALYZER_H
