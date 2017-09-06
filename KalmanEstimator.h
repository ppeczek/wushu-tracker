//
// Created by ppeczek on 8/18/17.
//

#ifndef WUSHUVIDEOANALYZER_KALMANESTIMATOR_H
#define WUSHUVIDEOANALYZER_KALMANESTIMATOR_H

#include "Commons.h"
#include "Settings.h"


class KalmanEstimator {
private:
    cv::KalmanFilter KF;
    cv::Mat_<float> measurement;
    cv::Mat estimated;

public:
    KalmanEstimator() {
        // Kalman Filter initialization
        KF = cv::KalmanFilter(4, 2, 0);
        KF.transitionMatrix = (cv::Mat_<float>(4, 4) << 1,0,1,0,  0,1,0,1,  0,0,1,0,  0,0,0,1);
        measurement = cv::Mat_<float>(2, 1);
        measurement.setTo(cv::Scalar(0));

        setIdentity(KF.measurementMatrix);
        setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-5));
        setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-3));
        setIdentity(KF.errorCovPost, cv::Scalar::all(.1));
    }

    void setMeasurement(const cv::Point &coordinates) {
        measurement(0) = coordinates.x;
        measurement(1) = coordinates.y;
    }

    cv::Mat predict() {
        return KF.predict();
    }

    cv::Mat correct(const cv::Point& point) {
        measurement(0) = point.x;
        measurement(1) = point.y;
        return KF.correct(measurement);
    }
};


#endif //WUSHUVIDEOANALYZER_KALMANESTIMATOR_H
