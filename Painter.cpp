//
// Created by ppeczek on 8/18/17.
//

#include "Painter.h"
#include "AnalysisResultPoint.h"


void Painter::drawPath(AnalyzerSettings& settings, String inputPath, String outputPathLines, Scalar pathColor) {
    Mat pattern = imread(settings.patternPath);
    Mat pathsImage = Mat::zeros(Size(Settings::outputImageWidth, Settings::outputImageHeight), CV_8UC3);
    AnalysisResultPoint resultPoint;

    for (int y=0; y<pathsImage.rows; ++y) {
        for (int x=0; x<pathsImage.cols; ++x) {
            pathsImage.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
        }
    }

    std::fstream infile(inputPath);
    bool firstPoint = true;

    Point oldPoint;
    while (infile >> resultPoint) {
        if (resultPoint.getCoordinates() != Commons::nullPoint) {
            circle(pathsImage, resultPoint.getCoordinates(), 0, pathColor);
            if (!firstPoint) {
                line(pathsImage, resultPoint.getCoordinates(), oldPoint, pathColor, 2);
            }
            oldPoint = resultPoint.getCoordinates();
            firstPoint = false;
        }
        else {
            cout << "-1" << endl;
        }
    }

//    pathsImage = Commons::fourPointsTransformation(pathsImage, corners);

    imwrite(outputPathLines, pathsImage);
}

void Painter::drawMeasuredPath(AnalyzerSettings& settings) {
    drawPath(settings, settings.localizationsPath, settings.pathsImagePath, Settings::redColor);
}

void Painter::drawKalmanPath(AnalyzerSettings& settings) {
    drawPath(settings, settings.localizationsKalmanPath, settings.pathsImageKalmanPath, Settings::greenColor);
}
