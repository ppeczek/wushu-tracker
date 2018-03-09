//
// Created by ppeczek on 7/21/17.
//

#ifndef WUSHUVIDEOANALYZER_SETTINGS_H
#define WUSHUVIDEOANALYZER_SETTINGS_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"


class Settings {
public:
    // ANALYSIS SETTINGS
    const static std::string version;

    // analysis mode
    const static bool debug;
    const static bool showDetailText;
    const static bool showBoundaries;

    // analyzer pattern, image settings
    const static int nFrames;
    const static int imageWidth;

    // operations factors
    const static int thresholdFactor;
    const static int blurFactor;

    // output image settings
    // 14m x 8m -> platform size
    const static int outputImageWidth;
    const static int outputImageHeight;


    // FILES SETTINGS

    // file extensions
    const static std::string videoExt;
    const static std::string imgExt;
    const static std::string resultsExt;

    // file names
    const static std::string videoName;
    const static std::string patternName;
    const static std::string resultsName;
    const static std::string pointsName;
    const static std::string pathsName;

    const static std::string minifyName;
    const static std::string kalmanName;

    // colors
    const static cv::Scalar redColor;
    const static cv::Scalar blueColor;
    const static cv::Scalar greenColor;
    const static cv::Scalar yellowColor;
    const static cv::Scalar whiteColor;
    const static cv::Scalar blackColor;

private:
    Settings() {};
};

#endif //WUSHUVIDEOANALYZER_SETTINGS_H
