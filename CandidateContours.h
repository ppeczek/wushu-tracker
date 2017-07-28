//
// Created by ppeczek on 7/24/17.
//

#ifndef WUSHUVIDEOANALYZER_CANDIDATECONTOURS_H
#define WUSHUVIDEOANALYZER_CANDIDATECONTOURS_H

#include <iostream>

#include "Settings.h"
#include "Commons.h"


class CandidateContours {
private:
    vector<Point> contour;
    Moments mu;
    Point mc;

    vector<Point> relative_points;
    double relative_points_min_diff;
    Point last_position;

    double score;
    double contour_size;
    double last_position_distance;
    double corner_distances;
    Point vectorCenter;

    double calculateScore() {
        corner_distances = 0;
        last_position_distance = (last_position == Point(-1, -1)) ? 1 : norm(mc - last_position);
        contour_size = contour.size();
        vectorCenter = Point(0, 0);

        for (auto&& corner: relative_points) {
            vectorCenter += mc - corner;
        }
        corner_distances = norm(vectorCenter);
        if (last_position_distance == 0) {
            last_position_distance = 1;
        }
        return (corner_distances * last_position_distance) / contour_size;
    }

public:
    CandidateContours() {
        contour = vector<Point>();
        relative_points = vector<Point>();
        score = 0;
    }

    CandidateContours(vector<Point>& c, vector<Point>& rp, const double rpmd, const Point& lp) :
            contour(c), relative_points(rp), relative_points_min_diff(rpmd), last_position(lp) {
        mu = moments(c, false);
        mc = Point((int)(mu.m10/mu.m00), (int)(mu.m01/mu.m00));
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

    const Moments &getMu() const {
        return mu;
    }

    const Point &getMc() const {
        return mc;
    }

    double getContourSize() const {
        return contour_size;
    }

    double getLastPositionDistance() const {
        return last_position_distance;
    }

    double getCornerDistances() const {
        return corner_distances;
    }

    const Point &getVectorCenter() const {
        return vectorCenter;
    }

};


#endif //WUSHUVIDEOANALYZER_CANDIDATECONTOURS_H
