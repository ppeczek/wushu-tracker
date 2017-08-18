//
// Created by ppeczek on 7/24/17.
//

#ifndef WUSHUVIDEOANALYZER_CANDIDATECONTOURS_H
#define WUSHUVIDEOANALYZER_CANDIDATECONTOURS_H

#include <iostream>

#include "Settings.h"
#include "Commons.h"
#include "Platform.h"


class CandidateContours {
private:
    // contour parameters
    vector<Point> contour;
    Moments mu;
    Point mc;
    Point bottomPixel;
    Rect bbox;

    Point lastPosition;
    Mat transformation;
    Point transformatedCoordinates;

    // score
    double score;
    double contourSize;
    double lastPositionDistance;
    double onPlatform;
    Point vectorCenter;

    double calculateScore() {
        onPlatform = Commons::isTransformatedPointValid(transformatedCoordinates) ? 1 : 100;
        lastPositionDistance = (lastPosition == Commons::nullPoint) ? 1 : 10 * norm(transformatedCoordinates - lastPosition);
        contourSize = contour.size();

        if (lastPositionDistance == 0) {
            lastPositionDistance = 1;
        }
        return contourSize / (onPlatform * lastPositionDistance);
    }

public:
    CandidateContours() {
        contour = vector<Point>();
        score = 0;
    }

    CandidateContours(vector<Point>& c, const Platform& platform, const Point& lp) :
            contour(c), lastPosition(lp), transformation(platform.getTransformation()) {
        mu = moments(c, false);
        mc = Point((int)(mu.m10/mu.m00), (int)(mu.m01/mu.m00));
        bbox = boundingRect(contour);
        bottomPixel = Point((int)(bbox.x + bbox.width * 0.5), bbox.y + bbox.height);
        transformatedCoordinates = platform.platformPerspectiveTransformation(bottomPixel);

        score = calculateScore();
    }

    friend double compare(const CandidateContours& lhs, const CandidateContours& rhs) {
        return lhs.score - rhs.score;
    }

    friend bool operator==(const CandidateContours& lhs, const CandidateContours& rhs) {
        return compare(lhs, rhs) == 0;
    }

    friend bool operator!=(const CandidateContours& lhs, const CandidateContours& rhs) {
        return compare(lhs, rhs) != 0;
    }

    friend bool operator<(const CandidateContours& lhs, const CandidateContours& rhs) {
        return compare(lhs, rhs) < 0;
    }

    friend bool operator>(const CandidateContours& lhs, const CandidateContours& rhs) {
        return compare(lhs, rhs) > 0;
    }

    friend bool operator<=(const CandidateContours& lhs, const CandidateContours& rhs) {
        return compare(lhs, rhs) <= 0;
    }

    friend bool operator>=(const CandidateContours& lhs, const CandidateContours& rhs) {
        return compare(lhs, rhs) >= 0;
    }

    const vector<Point> &getContour() const {
        return contour;
    }

    const Rect &getBbox() const {
        return bbox;
    }

    const Point &getBottomPixel() const {
        return bottomPixel;
    }

    const Point &getTransformatedCoordinates() const {
        return transformatedCoordinates;
    }

    const Point &getMc() const {
        return mc;
    }

    double getContourSize() const {
        return contourSize;
    }

    double getLastPositionDistance() const {
        return lastPositionDistance;
    }

    const Point &getVectorCenter() const {
        return vectorCenter;
    }

};


#endif //WUSHUVIDEOANALYZER_CANDIDATECONTOURS_H
