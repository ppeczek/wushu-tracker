//
// Created by ppeczek on 8/18/17.
//

#ifndef WUSHUVIDEOANALYZER_PROCESSEDIMAGE_H
#define WUSHUVIDEOANALYZER_PROCESSEDIMAGE_H

#include "Settings.h"
#include "CandidateContours.h"
#include "KalmanEstimator.h"
#include "Platform.h"
#include "AnalyzerSettings.h"

class ProcessedImage {
private:
    Mat image;

    // contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    vector<CandidateContours> candidateContours;
    CandidateContours athleteContours;

    // moments
    Point2f mc;
    Rect bbox;

    // coordinates in given image
    Point lastCoordinates;
    Point currentCoordinates;

    double detectionTime;

public:
    ProcessedImage() {
        currentCoordinates = Commons::nullPoint;
        lastCoordinates = Commons::nullPoint;
    };

    explicit ProcessedImage(const Mat& img) : image(img) {};

    void myBlur(const Mat& src, Mat& dst);
    Point detect(const Mat& frame, const Mat& pattern, const Platform& platform);
    Mat debugCameraImage(const AnalyzerSettings& settings, const Platform& platform, const double& fps);
    Mat debugPlatformImage(const AnalyzerSettings& settings, const Platform& platform, const Point& transformatedCoordinates, const Point& transformatedKalmanCoordinates);
    void drawBoundaries(Mat&, const AnalyzerSettings&, const Platform&, Scalar, int);

    const Mat &getImage() const;
    void setImage(const Mat &image);
    const Rect &getBbox() const;
    void setBbox(const Rect &bbox);
    double getDetectionTime() const;
    void setDetectionTime(double detectionTime);
};


#endif //WUSHUVIDEOANALYZER_PROCESSEDIMAGE_H
