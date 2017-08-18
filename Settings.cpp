//
// Created by ppeczek on 7/21/17.
//

#include "Settings.h"


const bool Settings::debug = false;
const bool Settings::showDetailText = false;
const bool Settings::showBoundaries = false;

const int Settings::nFrames = 10;
const int Settings::imageWidth = 720;

const int Settings::thresholdFactor = 15;
const int Settings::blurFactor = 5;

const int Settings::outputImageWidth = 700;
const int Settings::outputImageHeight = 400;

const String Settings::videoExt = "mp4";
const String Settings::imgExt = "png";
const String Settings::resultsExt = "dat";

const String Settings::videoName = "video";
const String Settings::patternName = "pattern";
const String Settings::resultsName = "result";
const String Settings::pointsName = "points";
const String Settings::pathsName = "paths";

const String Settings::minifyName = "min";
const String Settings::kalmanName = "kalman";

const Scalar Settings::redColor = Scalar(61, 61, 255);
const Scalar Settings::blueColor = Scalar(255, 72, 16);
const Scalar Settings::greenColor = Scalar(69, 212, 58);
const Scalar Settings::whiteColor = Scalar(255, 255, 255);
const Scalar Settings::blackColor = Scalar(0, 0, 0);
const Scalar Settings::yellowColor = Scalar(255, 0, 0);
