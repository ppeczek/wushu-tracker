//
// Created by ppeczek on 8/18/17.
//

#ifndef WUSHUVIDEOANALYZER_KALMANESTIMATOR_H
#define WUSHUVIDEOANALYZER_KALMANESTIMATOR_H

#include "Commons.h"
#include "Settings.h"


class KalmanEstimator {
private:
    KalmanFilter KF;
    Mat_<float> measurement;
    Mat estimated;

public:
    KalmanEstimator() {
        // Kalman Filter initialization
        KF = KalmanFilter(4, 2, 0);
        KF.transitionMatrix = (Mat_<float>(4, 4) << 1,0,1,0,  0,1,0,1,  0,0,1,0,  0,0,0,1);
        measurement = Mat_<float>(2, 1);
        measurement.setTo(Scalar(0));

        setIdentity(KF.measurementMatrix);
        setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-3));
        setIdentity(KF.errorCovPost, Scalar::all(.1));
    }

    void setMeasurement(const Point &coordinates) {
        measurement(0) = coordinates.x;
        measurement(1) = coordinates.y;
    }

    Mat predict() {
        return KF.predict();
    }

    Mat correct(const Point& point) {
        measurement(0) = point.x;
        measurement(1) = point.y;
        return KF.correct(measurement);
    }
};


#endif //WUSHUVIDEOANALYZER_KALMANESTIMATOR_H
