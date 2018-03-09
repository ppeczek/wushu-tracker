//
// Created by ppeczek on 8/18/17.
//

#ifndef WUSHUVIDEOANALYZER_PLATFORM_H
#define WUSHUVIDEOANALYZER_PLATFORM_H

#include "Settings.h"
#include <iostream>


class Platform {
private:
    std::vector<cv::Point> corners;

    // platform transformations
    cv::Mat transformation;

    cv::Mat createPerspectiveTransformation(const std::vector<cv::Point>& corners);
    cv::Mat platformWarpPerspective(const cv::Mat& img);

public:
    Platform(std::vector<cv::Point> cs) : corners(cs) {
        // initializing perspective Mat for further perspective transformations
        // can be created at any time with given corners
        transformation = createPerspectiveTransformation(cs);
    };

    Platform(const Platform& platform) : corners(platform.corners), transformation(platform.transformation) {
        // initializing perspective Mat for further perspective transformations
        // can be created at any time with given corners
        transformation = createPerspectiveTransformation(platform.corners);
    };

    ~Platform() {};

    void resize(float factor);
    cv::Point platformPerspectiveTransformation(const cv::Point& point) const;

    const std::vector<cv::Point> &getCorners() const;
    const cv::Mat &getTransformation() const;
};


#endif //WUSHUVIDEOANALYZER_PLATFORM_H
