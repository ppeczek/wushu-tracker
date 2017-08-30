//
// Created by ppeczek on 7/20/17.
//

#ifndef WUSHUVIDEOANALYZER_COMMONS_H
#define WUSHUVIDEOANALYZER_COMMONS_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/video/tracking.hpp"

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "Settings.h"

#include <iostream>
#include <iomanip>

#include <cassert>

using namespace cv;
using namespace std;


class Commons {
public:
    const static Point nullPoint;

    // font settings
    const static int fontFace;
    const static double fontScale;
    const static int thickness;
    const static int baseline;

    static Mat medianMerge(vector<Mat> frames);
    static void resetPoint(Point& point);
    static String formatDouble(double val);
    static bool isTransformatedPointValid(const Point& point);

private:
    Commons() {};
};


#endif //WUSHUVIDEOANALYZER_COMMONS_H
