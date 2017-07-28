//
// Created by ppeczek on 7/20/17.
//

#include "Commons.h"


Mat Commons::medianMerge(vector<Mat> frames) {
    Mat result;
    bool firstFrame = true;

    int channels = 0;
    int nRows = 0;
    int nCols = 0;
    int nFrames = frames.size();

    for (auto&& frame : frames) {
        if (firstFrame) {
            // copy first frame to result
            result = frame;

            // save image parameters
            channels = frame.channels();
            nRows = frame.rows;
            nCols = frame.cols;

            firstFrame = false;
        }
        else {
            assert(frame.channels() == channels);
            assert(frame.rows == nRows);
            assert(frame.cols == nCols);
        }
        cvtColor(frame, frame, CV_BGR2HSV);
    }

    vector<int> h(nFrames);
    vector<int> s(nFrames);
    vector<int> v(nFrames);
    vector<long> window(nFrames);

    switch (channels) {
        case 1:
            break;
        case 3:
            for (int y=0; y< nRows; ++y) {
                for (int x=0; x< nCols; ++x) {
                    int counter = 0;
                    for (auto&& frame : frames) {
                        h[counter] = frame.at<Vec3b>(y, x)[0];
                        s[counter] = frame.at<Vec3b>(y, x)[1];
                        v[counter] = frame.at<Vec3b>(y, x)[2];
                        ++counter;
                    }
                    sort(h.begin(), h.end());
                    sort(s.begin(), s.end());
                    sort(v.begin(), v.end());
                    result.at<Vec3b>(y, x)[0] = h[nFrames/2];
                    result.at<Vec3b>(y, x)[1] = s[nFrames/2];
                    result.at<Vec3b>(y, x)[2] = v[nFrames/2];
                }
            }
            break;
        default:
            throw;
    }

    cvtColor(result, result, CV_HSV2BGR);
    return result;
}

Mat Commons::fourPointsWarpTransformation(Mat img, vector<Point> corners) {
    // tl -> corners[0]
    // tr -> corners[1]
    // br -> corners[2]
    // bl -> corners[3]

    // 14m x 8m -> platform size
//    int maxWidth = 1400;
//    int maxHeight = 800;
    int maxWidth = 700;
    int maxHeight = 400;
//    double widthA = norm(corners[0] - corners[1]);
//    double widthB = norm(corners[2] - corners[3]);
//    int maxWidth = (int)max(widthA, widthB);
//
//    double heightA = norm(corners[0] - corners[3]);
//    double heightB = norm(corners[1] - corners[2]);
//    int maxHeight = (int)max(heightA, heightB);

    Point2f tl(0, 0), tr(maxWidth, 0), br(maxWidth, maxHeight), bl(0, maxHeight);

    Point2f srcPoints[4] = {corners[0], corners[1], corners[2], corners[3]};
    Point2f dstPoints[4] = {tl, tr, br, bl};

    Mat result(maxWidth, maxHeight, CV_8UC3);
    Mat transformation = getPerspectiveTransform(srcPoints, dstPoints);
    warpPerspective(img, result, transformation, Size(maxWidth, maxHeight));
    return result;
}

Mat Commons::fourPointsPerspectiveTransformation(vector<Point> points, vector<Point> corners){
    // tl -> corners[0]
    // tr -> corners[1]
    // br -> corners[2]
    // bl -> corners[3]

    // 14m x 8m -> platform size
//    int maxWidth = 1400;
//    int maxHeight = 800;
    int maxWidth = 700;
    int maxHeight = 400;

    Point2f tl(0, 0), tr(maxWidth, 0), br(maxWidth, maxHeight), bl(0, maxHeight);

    Point2f srcPoints[4] = {corners[0], corners[1], corners[2], corners[3]};
    Point2f dstPoints[4] = {tl, tr, br, bl};

    Mat transformation = getPerspectiveTransform(srcPoints, dstPoints);
    Mat result(points);
    perspectiveTransform(Mat(points), result, transformation);
    return result;
}