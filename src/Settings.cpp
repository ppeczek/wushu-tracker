//
// Created by ppeczek on 7/21/17.
//

#include "Settings.h"


const std::string Settings::version = "0.0.1";

const bool Settings::debug = false;
const bool Settings::showDetailText = false;
const bool Settings::showBoundaries = false;

const int Settings::nFrames = 10;
const int Settings::imageWidth = 720;

const int Settings::thresholdFactor = 15;
const int Settings::blurFactor = 5;

const int Settings::outputImageWidth = 700;
const int Settings::outputImageHeight = 400;

const std::string Settings::videoExt = "mp4";
const std::string Settings::imgExt = "png";
const std::string Settings::resultsExt = "dat";

const std::string Settings::videoName = "video";
const std::string Settings::patternName = "pattern";
const std::string Settings::resultsName = "result";
const std::string Settings::pointsName = "points";
const std::string Settings::pathsName = "paths";

const std::string Settings::minifyName = "min";
const std::string Settings::kalmanName = "kalman";

const cv::Scalar Settings::redColor = cv::Scalar(61, 61, 255);
const cv::Scalar Settings::blueColor = cv::Scalar(255, 72, 16);
const cv::Scalar Settings::greenColor = cv::Scalar(69, 212, 58);
const cv::Scalar Settings::whiteColor = cv::Scalar(255, 255, 255);
const cv::Scalar Settings::blackColor = cv::Scalar(0, 0, 0);
const cv::Scalar Settings::yellowColor = cv::Scalar(255, 0, 0);
