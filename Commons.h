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

#include <iostream>

using namespace cv;
using namespace std;


class Commons {
public:
    static Mat medianMerge(vector<Mat> frames);

private:
    Commons() {};

};


#endif //WUSHUVIDEOANALYZER_COMMONS_H
