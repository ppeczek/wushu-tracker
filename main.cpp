//
// Created by ppeczek on 5/12/17.
//
#include "VideoAnalyzer.cpp"

//    10th Pan American Wushu Championships Men's Changquan - 3rd Set - Rolando Lee (Gold)
//    "PAWC-1";
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

//    10th Pan American Wushu Championships Women's Changquan - 3rd Set - Emily Hwang (Gold)
//    "PAWC-2";
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

//    10th Pan American Wushu Championships Mens Changquan - Optional - Ryan Tang
//    "PAWC-3";
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

//    10th Pan American Wushu Championships Mens Nanquan - 3rd Set - Anthony Chen (Silver)
//    "PAWC-4";
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

//    10th Pan American Wushu Championships Women's Changquan - 3rd Set - Dusty Schmidt (Silver)
//    "PAWC-5"
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

int main() {
    clock_t start = clock();
    vector<VideoAnalyzer> analyzers;

    // corners order: top-left, top-right, bottom-right, bottom-left

    // 12th WWC videos
    Point a_wwc(-90, 565), b_wwc(788, 460), c_wwc(1310, 480), d_wwc(1010, 725);
    vector<Point> corners {a_wwc, b_wwc, c_wwc, d_wwc};
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[12-WWC-1]"), corners));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[12-WWC-2]"), corners));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[12-WWC-3]"), corners));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[12-WWC-4]"), corners));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[12-WWC-5]"), corners));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[12-WWC-6]"), corners));
//
////    // PAWC videos
    Point a_pawc(0, 505), b_pawc(880, 465), c_pawc(1260, 570), d_pawc(140, 700);
    corners.clear();
    corners = {a_pawc, b_pawc, c_pawc, d_pawc};
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-1"), corners));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-2"), corners));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-3"), corners));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-4"), corners));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-5"), corners));

    int i = 0;
    for (auto&& va : analyzers) {
        cout << "Video " << ++i << endl;
        va.analyze();

//        va.drawMeasuredPath(true);
//        va.drawKalmanPath(true);
    }

    double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Program duration: " << duration << endl;

    return 0;
}