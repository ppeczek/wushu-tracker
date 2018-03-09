//
// Created by ppeczek on 8/18/17.
//

#include "ProcessedImage.h"
#include "AnalysisResultPoint.h"
#include "Painter.h"


void ProcessedImage::myBlur(const Mat& src, Mat& dst) {
    resize(src, dst, Size(), 0.1, 0.1, INTER_CUBIC);
    resize(src, dst, Size(), 10, 10, INTER_CUBIC);
//        medianBlur(src, dst, settings.blurFactor);
//        GaussianBlur(src, dst, Size(settings.blurFactor, settings.blurFactor)); TODO: FIXME
//        blur(src, dst, settings.blurFactor);
}

Point ProcessedImage::detect(const Mat& frame, const Mat& pattern, const Platform& platform) {
    Mat result;
    clock_t start = clock();
    ProcessedImage::frame = frame;
    candidateContours.clear();
    lastCoordinates = currentCoordinates;

    // abs diff so it doesn't matter whether background is lighter or darker
    absdiff(pattern, frame, result);
    frameDiffMat = result;

    // blur
    myBlur(result, result);

    // threshold
    cvtColor(result, result, CV_BGR2GRAY);
    threshold(result, result, Settings::thresholdFactor, 255, 0);
    thresholdedMat = result;

    findContours(result, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    for(auto&& contour: contours) {
        candidateContours.emplace_back(CandidateContours(contour, platform, lastCoordinates));
    }

    Commons::resetPoint(currentCoordinates);
    if (!candidateContours.empty()) {
        athleteContours = *max_element(candidateContours.begin(), candidateContours.end());
        mc = athleteContours.getMc();
        bbox = athleteContours.getBbox();
        currentCoordinates = athleteContours.getBottomPixel();
        return athleteContours.getTransformatedCoordinates();
    }
    detectionTime = (clock() - start) / (double) CLOCKS_PER_SEC;

    return Commons::nullPoint;
}

Mat ProcessedImage::debugCameraImage(const Platform& platform, const double& fps) {
    cameraViewMat = frame.clone();

    drawBoundaries(cameraViewMat, platform, Settings::blueColor, 2);
    circle(cameraViewMat, currentCoordinates, 3, Settings::redColor, -1);
    for (int i = 0; i < (int)contours.size(); ++i) {
        Scalar contourColor =
                (contours[i] == athleteContours.getContour()) ? Settings::redColor : Settings::whiteColor;
        drawContours(cameraViewMat, contours, i, contourColor);
    }

    // text display
    if (Settings::showDetailText) {
        for (auto&& candidate: candidateContours) {
            Scalar contourColor =
                    (candidate.getContour() == athleteContours.getContour()) ?
                    Settings::redColor : Settings::whiteColor;
            String contourText =  "c: " + Commons::formatDouble(candidate.getContourSize()) + ", " +
                    // "cd: " + formatDouble(candidate.getCornerDistances()) + ", " +
                    "cd: [" + to_string(candidate.getVectorCenter().x) + ", " + to_string(candidate.getVectorCenter().y) + "], " +
                    "lp: " + Commons::formatDouble(candidate.getLastPositionDistance());
            Point textOrg(candidate.getMc().x + 10, candidate.getMc().y + 10);
            putText(cameraViewMat, contourText, textOrg, Commons::fontFace, Commons::fontScale, contourColor, Commons::thickness, 8);
        }
    }

    // fps display
    String fpsText = "FPS: " + to_string((int)fps);
    Point fpsOrg(20, 20);
    putText(cameraViewMat, fpsText, fpsOrg, Commons::fontFace, 0.5, Settings::greenColor, Commons::thickness, 8);

    return cameraViewMat;
}

Mat ProcessedImage::debugPlatformImage(const Point &transformatedCoordinates, const Point &transformatedKalmanCoordinates) {
    platformViewMat = Mat::zeros(Size(Settings::outputImageWidth, Settings::outputImageHeight), CV_8UC3);

    for (int y=0; y<platformViewMat.rows; ++y) {
        for (int x=0; x<platformViewMat.cols; ++x) {
            platformViewMat.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
        }
    }

    circle(platformViewMat, transformatedCoordinates, 3, Settings::redColor, -1);
    circle(platformViewMat, transformatedKalmanCoordinates, 3, Settings::greenColor, -1);

    return platformViewMat;
}

void ProcessedImage::drawBoundaries(Mat& img, const Platform& platform, Scalar color=Settings::blueColor, int lineWidth=2) {
    auto max = (int)platform.getCorners().size();
    for (int i=0; i<max; ++i) {
        int nextIndex = (i+1) % max;
        Point a = platform.getCorners()[i];
        Point b = platform.getCorners()[nextIndex];
        line(img, a, b, color, lineWidth);

        String text = to_string(i + 1);
        Point textOrg(a.x, a.y);
        putText(img, text, textOrg, Commons::fontFace, Commons::fontScale, Settings::whiteColor, Commons::thickness, 8);
    }
}

void ProcessedImage::createSnapshot() {
    cout << "Create a snapshot" << endl;

    imwrite("snapshots/snapshot-001-image.png", frame);
    imwrite("snapshots/snapshot-001-camera.png", cameraViewMat);
    imwrite("snapshots/snapshot-001-platform.png", platformViewMat);
    imwrite("snapshots/snapshot-001-diff.png", frameDiffMat);
    imwrite("snapshots/snapshot-001-thresholded.png", thresholdedMat);
}

const Mat &ProcessedImage::getFrame() const {
    return frame;
}

void ProcessedImage::setFrame(const Mat &frame) {
    ProcessedImage::frame = frame;
}

const Rect &ProcessedImage::getBbox() const {
    return bbox;
}

void ProcessedImage::setBbox(const Rect &bbox) {
    ProcessedImage::bbox = bbox;
}

double ProcessedImage::getDetectionTime() const {
    return detectionTime;
}

void ProcessedImage::setDetectionTime(double detectionTime) {
    ProcessedImage::detectionTime = detectionTime;
}
