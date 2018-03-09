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
    cv::Mat frame;
    cv::Mat cameraViewMat;
    cv::Mat platformViewMat;
    cv::Mat frameDiffMat;
    cv::Mat thresholdedMat;

    // contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    std::vector<CandidateContours> candidateContours;
    CandidateContours athleteContours;

    // moments
    cv::Point2f mc;
    cv::Rect bbox;

    // coordinates in given image
    cv::Point lastCoordinates;
    cv::Point currentCoordinates;

    double detectionTime;

public:
    ProcessedImage() {
        currentCoordinates = Commons::nullPoint;
        lastCoordinates = Commons::nullPoint;
    };

    ProcessedImage(const cv::Mat& img) : frame(img) {};

    void myBlur(const cv::Mat& src, cv::Mat& dst);
    cv::Point detect(const cv::Mat& frame, const cv::Mat& pattern, const Platform& platform);
    cv::Mat debugCameraImage(const Platform& platform, const double& fps);
    cv::Mat debugPlatformImage(const cv::Point& transformatedCoordinates, const cv::Point& transformatedKalmanCoordinates);
    void drawBoundaries(cv::Mat&, const Platform&, cv::Scalar, int);

    void createSnapshot();

    const cv::Mat &getFrame() const;
    void setFrame(const cv::Mat &frame);
    const cv::Rect &getBbox() const;
    void setBbox(const cv::Rect &bbox);
    double getDetectionTime() const;
    void setDetectionTime(double detectionTime);
};


#endif //WUSHUVIDEOANALYZER_PROCESSEDIMAGE_H
