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
    std::vector<cv::Point> contour;
    cv::Moments mu;
    cv::Point mc;
    cv::Point bottomPixel;
    cv::Rect bbox;

    cv::Point lastPosition;
    cv::Mat transformation;
    cv::Point transformatedCoordinates;

    // score
    double score;
    double contourSize;
    double lastPositionDistance;
    double onPlatform;
    cv::Point vectorCenter;

    double calculateScore() {
        onPlatform = Commons::isTransformatedPointValid(transformatedCoordinates) ? 1 : 100;
        lastPositionDistance = (lastPosition == Commons::nullPoint) ? 1 : 100 * norm(transformatedCoordinates - lastPosition);
        contourSize = contour.size();

        if (lastPositionDistance == 0) {
            lastPositionDistance = 1;
        }
        return contourSize / (onPlatform * lastPositionDistance);
    }

public:
    CandidateContours() {
        contour = std::vector<cv::Point>();
        score = 0;
    }

    CandidateContours(std::vector<cv::Point>& c, const Platform& platform, const cv::Point& lp) :
            contour(c), lastPosition(lp), transformation(platform.getTransformation()) {
        mu = moments(c, false);
        mc = cv::Point((int)(mu.m10/mu.m00), (int)(mu.m01/mu.m00));
        bbox = boundingRect(contour);
        bottomPixel = cv::Point((int)(bbox.x + bbox.width * 0.5), bbox.y + bbox.height);
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

    const std::vector<cv::Point> &getContour() const {
        return contour;
    }

    const cv::Rect &getBbox() const {
        return bbox;
    }

    const cv::Point &getBottomPixel() const {
        return bottomPixel;
    }

    const cv::Point &getTransformatedCoordinates() const {
        return transformatedCoordinates;
    }

    const cv::Point &getMc() const {
        return mc;
    }

    double getContourSize() const {
        return contourSize;
    }

    double getLastPositionDistance() const {
        return lastPositionDistance;
    }

    const cv::Point &getVectorCenter() const {
        return vectorCenter;
    }

};


#endif //WUSHUVIDEOANALYZER_CANDIDATECONTOURS_H
