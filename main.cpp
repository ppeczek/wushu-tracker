//
// Created by ppeczek on 5/12/17.
//
#include "VideoAnalyzer.cpp"

//    10th Pan American Wushu Championships Men's Changquan - 3rd Set - Rolando Lee (Gold)
//    "1";
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

//    10th Pan American Wushu Championships Women's Changquan - 3rd Set - Emily Hwang (Gold)
//    "2";
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

//    10th Pan American Wushu Championships Mens Changquan - Optional - Ryan Tang
//    "3";
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

//    10th Pan American Wushu Championships Mens Nanquan - 3rd Set - Anthony Chen (Silver)
//    "4";
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

//    10th Pan American Wushu Championships Women's Changquan - 3rd Set - Dusty Schmidt (Silver)
//    "5"
//    Point pointA(0, 505);
//    Point pointB(140, 700);
//    Point pointC(1260, 570);
//    Point pointD(880, 465);

int main() {
    clock_t start = clock();
    vector<VideoAnalyzer> analyzers;
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1"), Point(0, 505), Point(140, 700), Point(1260, 570), Point(880, 465)));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("2"), Point(0, 505), Point(140, 700), Point(1260, 570), Point(880, 465)));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("3"), Point(0, 505), Point(140, 700), Point(1260, 570), Point(880, 465)));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("4"), Point(0, 505), Point(140, 700), Point(1260, 570), Point(880, 465)));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("5"), Point(0, 505), Point(140, 700), Point(1260, 570), Point(880, 465)));

    int i = 0;
    for (auto&& va : analyzers) {
        cout << "Video " << ++i << endl;
//        Commons::createPattern();
        va.analyze(true, true);
//        va.analyze();

//        va.drawMeasuredPath(true);
//        va.drawMeasuredMinPath(true);

//        va.applyKalmanFilter();
//        va.drawEstimatedPath(true);
    }

    clock_t duration = (clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Program duration: " << duration << endl;

    return 0;
}