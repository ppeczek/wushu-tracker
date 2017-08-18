//
// Created by ppeczek on 8/18/17.
//

#ifndef WUSHUVIDEOANALYZER_PAINTER_H
#define WUSHUVIDEOANALYZER_PAINTER_H

#include <fstream>

#include "Settings.h"
#include "AnalyzerSettings.h"
#include "Platform.h"


class Painter {
public:
    static void drawPath(AnalyzerSettings&, String, String, Scalar);
    static void drawMeasuredPath(AnalyzerSettings&);
    static void drawKalmanPath(AnalyzerSettings&);

private:
    Painter() {};
};


#endif //WUSHUVIDEOANALYZER_PAINTER_H
