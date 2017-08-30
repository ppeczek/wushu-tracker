//
// Created by ppeczek on 5/12/17.
//
#include <zconf.h>
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

    // 12th WWC videos #1
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[1]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[2]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[3]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[4]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[5]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[6]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));

    // 12th WWC videos #2
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[1]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[2]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[3]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[4]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[5]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[6]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[7]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));

    // 12th WWC videos #3
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[1]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[2]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[3]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[4]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[5]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[6]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[7]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[8]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));

    // 12th WWC videos #4
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[1]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[2]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[3]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[4]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[5]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[6]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[7]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));

    // 12th WWC videos #5
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[1]"), {Point(920, 420), Point(1450, 645), Point(-460, 625), Point(390, 420)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[2]"), {Point(920, 420), Point(1450, 645), Point(-460, 625), Point(390, 420)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[3]"), {Point(920, 420), Point(1450, 645), Point(-460, 625), Point(390, 420)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[4]"), {Point(955, 425), Point(1580, 700), Point(-570, 670), Point(390, 425)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[5]"), {Point(955, 425), Point(1580, 700), Point(-570, 670), Point(390, 425)}));

    // PAWC videos
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-1"), {Point(0, 505), Point(880, 465), Point(1260, 570), Point(140, 700)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-2"), {Point(10, 498), Point(880, 462), Point(1260, 565), Point(150, 695)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-3"), {Point(17, 488), Point(880, 458), Point(1260, 563), Point(155, 680)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-4"), {Point(17, 488), Point(880, 458), Point(1260, 563), Point(160, 685)}));
//    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-5"), {Point(8, 455), Point(875, 420), Point(1255, 525), Point(145, 650)}));

    int i = 0;
    for (auto&& va : analyzers) {
        cout << "Video " << ++i << endl;
        va.analyze(va.PPECZEK);
//        va.analyze(va.MIL);
//        va.analyze(va.BOOSTING);
//        va.analyze(va.KCF);
//        va.analyze(va.TLD);
//        va.analyze(va.MEDIANFLOW);
//        va.analyze(va.GOTURN);
        cout << va.getStatistics() << endl;
        cout << endl;

        va.drawPaths();
    }

    double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Program duration: " << duration << endl;

    return 0;
}