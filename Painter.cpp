//
// Created by ppeczek on 8/18/17.
//

#include "Painter.h"
#include "AnalysisResultPoint.h"


void Painter::drawPath(AnalyzerSettings& settings, std::string inputPath, std::string outputPathLines, cv::Scalar pathColor) {
    Mat pattern = imread(settings.patternPath);
    Mat pathsImage = Mat::zeros(Size(Settings::outputImageWidth, Settings::outputImageHeight), CV_8UC3);
    AnalysisResultPoint resultPoint;

    std::fstream infile(inputPath);
    bool firstPoint = true;

    Point oldPoint;
    Point avgPoint;

    for (int y=0; y<pathsImage.rows; ++y) {
        for (int x=0; x<pathsImage.cols; ++x) {
            pathsImage.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
        }
    }
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

    imwrite(outputPathLines, pathsImage);
}

void Painter::drawMeasuredPath(AnalyzerSettings& settings) {
    drawPath(settings, settings.localizationsPath, settings.pathsImagePath, Settings::redColor);
}

void Painter::drawKalmanPath(AnalyzerSettings& settings) {
    drawPath(settings, settings.localizationsKalmanPath, settings.pathsImageKalmanPath, Settings::greenColor);
}
