//
// Created by ppeczek on 8/18/17.
//

#include "Platform.h"


void Platform::resize(float factor) {
    for (auto&& corner: corners) {
        corner *= factor;
    }
    // initializing perspective Mat for further perspective transformations
    // can be created at any time with given corners
    transformation = createPerspectiveTransformation(corners);
}

Mat Platform::createPerspectiveTransformation(const vector<Point>& corners) {
    // tl -> corners[0]
    // tr -> corners[1]
    // br -> corners[2]
    // bl -> corners[3]

    Point2f tl(0, 0),
            tr(Settings::outputImageWidth, 0),
            br(Settings::outputImageWidth, Settings::outputImageHeight),
            bl(0, Settings::outputImageHeight);

    Point2f srcPoints[4] = {corners[0], corners[1], corners[2], corners[3]};
    Point2f dstPoints[4] = {tl, tr, br, bl};
    return getPerspectiveTransform(srcPoints, dstPoints);
}

Point Platform::platformPerspectiveTransformation(const Point& point) const {
    Mat points(1, 1, CV_32FC2, {(double)point.x, (double)point.y});
    perspectiveTransform(points, points, transformation);
    return Point((int)points.at<float>(0), (int)points.at<float>(1));
}

Mat Platform::platformWarpPerspective(const Mat& img) {
    Mat result(Settings::outputImageWidth, Settings::outputImageHeight, CV_8UC3);
    warpPerspective(img, result, transformation, Size(Settings::outputImageWidth, Settings::outputImageHeight));
    return result;
}

const vector<Point> &Platform::getCorners() const {
    return corners;
}

const Mat &Platform::getTransformation() const {
    return transformation;
}

