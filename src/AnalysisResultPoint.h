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


class AnalysisResultPoint {
private:
    Point coordinates;
    int timestamp;

public:
    AnalysisResultPoint() {};
    AnalysisResultPoint(const Point c, int t): coordinates(c), timestamp(t) {};

    Mat toMat() {
        return Mat(vector<Point>{coordinates});
    }

    friend std::ostream& operator<< (std::ostream &out, const AnalysisResultPoint& rp) {
        out << rp.getCoordinates().x << " " << rp.getCoordinates().y << " " << rp.getTimestamp();
        return out;
    };

    friend std::istream& operator>> (std::istream &in, AnalysisResultPoint& rp) {
        int x, y, ts;
        in >> x >> y >> ts;
        rp.setCoordinates(Point(x, y));
        rp.setTimestamp(ts);
        return in;
    };

    friend bool operator==(const AnalysisResultPoint& lhs, const AnalysisResultPoint& rhs) {
        return lhs.coordinates == rhs.coordinates && lhs.timestamp == rhs.timestamp;
    }

    friend bool operator!=(const AnalysisResultPoint& lhs, const AnalysisResultPoint& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const AnalysisResultPoint& lhs, const AnalysisResultPoint& rhs) {
        return lhs.timestamp < rhs.timestamp;
    }

    friend bool operator>(const AnalysisResultPoint& lhs, const AnalysisResultPoint& rhs) {
        return lhs.timestamp > rhs.timestamp;
    }

    friend bool operator<=(const AnalysisResultPoint& lhs, const AnalysisResultPoint& rhs) {
        return lhs.timestamp <= rhs.timestamp;
    }

    friend bool operator>=(const AnalysisResultPoint& lhs, const AnalysisResultPoint& rhs) {
        return lhs.timestamp >= rhs.timestamp;
    }

    const Point &getCoordinates() const {
        return coordinates;
    }

    int getTimestamp() const {
        return timestamp;
    }

    void setCoordinates(const Point &coordinates) {
        AnalysisResultPoint::coordinates = coordinates;
    }

    void setTimestamp(int timestamp) {
        AnalysisResultPoint::timestamp = timestamp;
    }

};


#endif //WUSHUVIDEOANALYZER_RESULTPOINT_H
