//
// Created by ppeczek on 7/21/17.
//

#ifndef WUSHUVIDEOANALYZER_SETTINGS_H
#define WUSHUVIDEOANALYZER_SETTINGS_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"

using namespace cv;
using namespace std;


class Settings {
public:
    const static String videoExt;
    const static String imgExt;
    const static String resultsExt;

    const static String videoName;
    const static String patternName;
    const static String resultsName;
    const static String pointsName;
    const static String pathsName;

    const static String minifyName;
    const static String kalmanName;

    const static Scalar redColor;
    const static Scalar blueColor;
    const static Scalar greenColor;
    const static Scalar whiteColor;
    const static Scalar blackColor;

private:
    Settings() {};
};

#endif //WUSHUVIDEOANALYZER_SETTINGS_H
