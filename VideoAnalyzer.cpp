//
// Created by ppeczek on 5/12/17.
//

#include "VideoAnalyzer.h"

int VideoAnalyzer::prepareAnalysis() {
    pattern = imread(settings.patternPath);
    cntr = 0;

    if (pattern.empty()) {
        cout << "create pattern";
        pattern = createPattern();
    }

    if (stat(settings.resultsPath.c_str(), &info) != 0) {
        mkdir(settings.resultsPath.c_str(), ACCESSPERMS);
    }

    // resizing
    settings.adjustResizeImageParameters(pattern);
    if (settings.doResize) {
        resize(pattern, pattern, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
        platform.resize(settings.resizeFactor);
    }

    return 0;
}

Mat VideoAnalyzer::createPattern() {
    vector<Mat> frames;

    VideoCapture cap;
    cap.open(settings.videoPath);

    if (!cap.isOpened()) {
        cout << "nima video" << endl;
        return Mat();
    }

    for (int j=0; j<Settings::nFrames; ++j) {
        String framePath = settings.framesPath + "frame_" + to_string(j+1) + ".png";
        cap.set(CAP_PROP_POS_FRAMES, j*200);
        Mat frame;
        cap >> frame;

        if(frame.empty()) {
            break;
        }

        frames.push_back(frame);
        imwrite(framePath, frame);
    }

    Mat pattern = Commons::medianMerge(frames);
    imwrite(settings.patternPath, pattern);

    return pattern;
}

void VideoAnalyzer::analyze() {
    VideoCapture cap(settings.videoPath);

    Mat frame;

    prepareAnalysis();

    while (true) {
        cap >> frame;

        if (frame.empty())
            break;

        if (settings.doResize) {
            resize(frame, frame, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
        }

        // detect
        transformatedCoordinates = processedImage.detect(frame, pattern, platform);
//        cout << transformatedCoordinates << endl;

        AnalysisResultPoint arp(transformatedCoordinates, cntr);
        buf << arp << endl;

        // Kalman Filter magic
        // predict phase
        Mat prediction = KE.predict();
        Mat estimated = (transformatedCoordinates != Commons::nullPoint) ? KE.correct(transformatedCoordinates) : prediction;

        transformatedKalmanCoordinates.x = estimated.at<float>(0);
        transformatedKalmanCoordinates.y = estimated.at<float>(1);
        arp.setCoordinates(transformatedKalmanCoordinates);
        arp.setTimestamp(cntr);
        resultKalmanPoints.emplace_back(arp);
        bufKalman << arp << endl;

        ++cntr;

        // DEBUG display mode
        if (Settings::debug) {
            imshow("platform view", processedImage.debugPlatformImage(settings, platform, transformatedCoordinates, transformatedKalmanCoordinates));
            imshow("camera view", processedImage.debugCameraImage(settings, platform));
            int c = waitKey(30);
            if(c == 'q' || c == 'Q' || (c & 255) == 27) {
                break;
            }
        }

    }

    ofstream resultFile;

    // detection results
    resultFile.open(settings.localizationsPath);
    resultFile << buf.str();
    resultFile.close();

    // kalman results
    resultFile.open(settings.localizationsKalmanPath);
    resultFile << bufKalman.str();
    resultFile.close();

}

void VideoAnalyzer::opencvAnalyze(TrackingModes trackingMode) {
    switch (trackingMode) {
        case MIL:
            opencvAnalyze(TrackerMIL::create());
            break;
        case BOOSTING:
            opencvAnalyze(TrackerBoosting::create());
            break;
        case KCF:
            opencvAnalyze(TrackerKCF::create());
            break;
        case TLD:
            opencvAnalyze(TrackerTLD::create());
            break;
        case MEDIANFLOW:
            opencvAnalyze(TrackerMedianFlow::create());
            break;
        case GOTURN:
            opencvAnalyze(TrackerGOTURN::create());
            break;
        default:
            cout << "Bad option";
            break;
    }
}

int VideoAnalyzer::opencvAnalyze(Ptr<Tracker> tracker) {
//    VideoCapture cap(settings.videoPath);
//    Mat frame;
//
//    prepareAnalysis();
//
//    cap >> frame;
//
//    if (frame.empty()) {
//        cout << "nima video";
//        return 1;
//    }
//
//    if (settings.doResize) {
//        resize(frame, frame, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
//    }
//
//    // detect
////    detect(frame);
//
//    // Define an initial bounding box
////    Rect2d trackingBbox(bbox);
//
//    // Initialize tracker with first frame and bounding box
//    tracker->init(frame, trackingBbox);
//
//    while (true) {
//        cap >> frame;
//
//        if (frame.empty())
//            break;
//
//        if (settings.doResize) {
//            resize(frame, frame, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
//        }
//
//        // Update tracking results
//        tracker->update(frame, trackingBbox);
//
//        // Draw bounding box
//        rectangle(frame, trackingBbox, Scalar( 255, 0, 0 ), 2, 1 );
//
//        // Display result
//        imshow("Tracking", frame);
//        int k = waitKey(1);
//        if(k == 27) break;
//    }

    return 0;
}

void VideoAnalyzer::drawPaths() {
    Painter::drawMeasuredPath(settings);
    Painter::drawKalmanPath(settings);
}
