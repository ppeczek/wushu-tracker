//
// Created by ppeczek on 5/12/17.
//

#include "VideoAnalyzer.h"

int VideoAnalyzer::prepareAnalysis() {
    pattern = imread(settings.patternPath);

    if (pattern.empty()) {
        cout << "create pattern";
        pattern = createPattern();
    }

    if (stat(settings.resultsPath.c_str(), &info) != 0) {
        mkdir(settings.resultsPath.c_str(), ACCESSPERMS);
    }

    if (stat("snapshots", &info) != 0) {
        mkdir("snapshots", ACCESSPERMS);
    }

    // resizing
    settings.adjustResizeImageParameters(pattern);
    if (settings.doResize) {
        resize(pattern, pattern, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
        platform.resize(settings.resizeFactor);
    }

    videoFPS = (int)cap.get(CV_CAP_PROP_FPS);
    videoFramesLength = (int)cap.get(CV_CAP_PROP_FRAME_COUNT);
    statistics.init(videoFPS, videoFramesLength);

    return 0;
}

Mat VideoAnalyzer::createPattern() {
    vector<Mat> frames;

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

    cap.set(CAP_PROP_POS_FRAMES, 0);
    Mat pattern = Commons::medianMerge(frames);
    imwrite(settings.patternPath, pattern);

    return pattern;
}

void VideoAnalyzer::analyze(TrackingMode trackingMode) {
    switch (trackingMode) {
        case PPECZEK:
            myAnalysisMethod();
            break;
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

void VideoAnalyzer::myAnalysisMethod() {
    Mat frame;
    int currentPos;
    int visibleFPS = 0;

    prepareAnalysis();

    while (true) {
        clock_t start = clock();
        cap >> frame;
        currentPos = (int)cap.get(CAP_PROP_POS_FRAMES);

        if (frame.empty()) break;

        if (settings.doResize) {
            resize(frame, frame, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
        }

        // detect
        transformatedCoordinates = processedImage.detect(frame, pattern, platform);

        AnalysisResultPoint arp(transformatedCoordinates, currentPos);
        if (Commons::isTransformatedPointValid(transformatedCoordinates)) {
            buf << arp << endl;
        }

        // Kalman Filter magic
        // predict phase
        Mat prediction = KE.predict();
        Mat estimated = (Commons::isTransformatedPointValid(transformatedCoordinates)) ? KE.correct(transformatedCoordinates) : prediction;

        transformatedKalmanCoordinates.x = estimated.at<float>(0);
        transformatedKalmanCoordinates.y = estimated.at<float>(1);
        arp.setCoordinates(transformatedKalmanCoordinates);
        arp.setTimestamp(currentPos);
        resultKalmanPoints.emplace_back(arp);
        bufKalman << arp << endl;

        double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
        double fps = 1 / duration;
        statistics.increment(fps);

        if (currentPos == 1) {
            visibleFPS = fps;
        }
        if (currentPos % videoFPS == 0) {
            visibleFPS = statistics.countPerSec();
        }

        // DEBUG display mode
        if (Settings::debug) {
            imshow("platform view", processedImage.debugPlatformImage(transformatedCoordinates, transformatedKalmanCoordinates));
            imshow("camera view", processedImage.debugCameraImage(platform, visibleFPS));
            int k = waitKey(30);
            if(k == 'q' || k == 'Q' || (k & 255) == 27) {
                break;
            }
            else if (k == 's' || k == 'S') {
                processedImage.createSnapshot();
            }
        }

    }

    statistics.finish();

    // save only if not DEBUG mode
    if (!Settings::debug) {
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
}

int VideoAnalyzer::opencvAnalyze(Ptr<Tracker> tracker) {
    Mat frame;
    int currentPos;
    int visibleFPS = 0;

    prepareAnalysis();

    cap >> frame;

    if (frame.empty()) {
        cout << "nima video";
        return 1;
    }

    if (settings.doResize) {
        resize(frame, frame, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
    }

    // detect
    processedImage.detect(frame, pattern, platform);

    // Define an initial bounding box
    Rect2d trackingBbox(processedImage.getBbox());

    // Initialize tracker with first frame and bounding box
    tracker->init(frame, trackingBbox);

    while (true) {
        clock_t start = clock();
        cap >> frame;
        currentPos = (int)cap.get(CAP_PROP_POS_FRAMES);

        if (frame.empty())
            break;

        if (settings.doResize) {
            resize(frame, frame, Size(), settings.resizeFactor, settings.resizeFactor, INTER_CUBIC);
        }

        // Update tracking results
        tracker->update(frame, trackingBbox);

        double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
        double fps = 1 / duration;
        statistics.increment(fps);

        if (currentPos == 1) {
            visibleFPS = fps;
        }
        if (currentPos % videoFPS == 0) {
            visibleFPS = statistics.countPerSec();
        }

        // DEBUG display mode
        if (Settings::debug) {
            // Draw bounding box
            rectangle(frame, trackingBbox, Scalar( 255, 0, 0 ), 2, 1 );

            // fps display
            String fpsText = "FPS: " + to_string((int)visibleFPS);
            Point fpsOrg(20, 20);
            putText(frame, fpsText, fpsOrg, Commons::fontFace, 0.5, Settings::greenColor, Commons::thickness, 8);

            imshow("Tracking", frame);
            int k = waitKey(30);
            if (k == 'q' || k == 'Q' || (k & 255) == 27) {
                break;
            }
        }
    }

    statistics.finish();

    return 0;
}

void VideoAnalyzer::drawPaths() {
    Painter::drawMeasuredPath(settings);
    Painter::drawKalmanPath(settings);
}

const AnalysisStatistics &VideoAnalyzer::getStatistics() const {
    return statistics;
}
