//
// Created by ppeczek on 7/28/17.
//

#ifndef WUSHUVIDEOANALYZER_RESULTPOINT_H
#define WUSHUVIDEOANALYZER_RESULTPOINT_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"

using namespace cv;
using namespace std;


class ResultPoint {
private:
    Point coordinates;
    int timestamp;

public:
    ResultPoint(Point c, int t): coordinates(c), timestamp(t) {};

    friend std::ostream& operator<< (std::ostream &out, const ResultPoint& rp) {
        out << rp.getCoordinates().x << " " << rp.getCoordinates().y << " " << rp.getTimestamp();
        return out;
    };

    friend bool operator==(const ResultPoint& lhs, const ResultPoint& rhs) {
        return lhs.coordinates == rhs.coordinates && lhs.timestamp == rhs.timestamp;
    }

    friend bool operator!=(const ResultPoint& lhs, const ResultPoint& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const ResultPoint& lhs, const ResultPoint& rhs) {
        return lhs.timestamp < rhs.timestamp;
    }

    friend bool operator>(const ResultPoint& lhs, const ResultPoint& rhs) {
        return lhs.timestamp > rhs.timestamp;
    }

    friend bool operator<=(const ResultPoint& lhs, const ResultPoint& rhs) {
        return lhs.timestamp <= rhs.timestamp;
    }

    friend bool operator>=(const ResultPoint& lhs, const ResultPoint& rhs) {
        return lhs.timestamp >= rhs.timestamp;
    }

    const Point &getCoordinates() const {
        return coordinates;
    }

    int getTimestamp() const {
        return timestamp;
    }

};


#endif //WUSHUVIDEOANALYZER_RESULTPOINT_H
