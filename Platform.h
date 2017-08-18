//
// Created by ppeczek on 8/18/17.
//

#ifndef WUSHUVIDEOANALYZER_PLATFORM_H
#define WUSHUVIDEOANALYZER_PLATFORM_H

#include "Settings.h"
#include <iostream>


class Platform {
private:
    vector<Point> corners;

    // platform transformations
    Mat transformation;

    Mat createPerspectiveTransformation(const vector<Point>& corners);
    Mat platformWarpPerspective(const Mat& img);

public:
    Platform(vector<Point> cs) : corners(cs) {
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
    Point platformPerspectiveTransformation(const Point& point) const;

    const vector<Point> &getCorners() const;
    const Mat &getTransformation() const;
};


#endif //WUSHUVIDEOANALYZER_PLATFORM_H
