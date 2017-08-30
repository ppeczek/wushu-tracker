//
// Created by ppeczek on 8/30/17.
//

#ifndef WUSHUVIDEOANALYZER_ANALYSISSTATISTICS_H
#define WUSHUVIDEOANALYZER_ANALYSISSTATISTICS_H

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>

using namespace std;


class AnalysisStatistics {
public:
    AnalysisStatistics() {
        init(0, 0);
    };
    AnalysisStatistics(const AnalysisStatistics& as) :
            videoFPS(as.videoFPS), videoFramesLength(as.videoFramesLength),
            avgFPS(as.avgFPS), analysisStart(as.analysisStart), analysisDuration(as.analysisDuration),
            counterFPS(as.counterFPS), visibleFPS(as.visibleFPS)
    {};
    ~AnalysisStatistics() {};

    void init(int vfps, int vfl) {
        videoFPS = vfps;
        videoFramesLength = vfl;

        avgFPS = 0;
        analysisStart = clock();
        analysisDuration = 0;

        counterFPS = 0;
        visibleFPS = 0;
    };

    void increment(double val) {
        avgFPS += val;
        counterFPS += val;
    };

    int countPerSec() {
        visibleFPS = counterFPS / videoFPS;
        counterFPS = 0;
        return visibleFPS;
    };

    void finish() {
        analysisDuration = (clock() - analysisStart) / (double) CLOCKS_PER_SEC;
        avgFPS /= videoFramesLength;
    }


    friend std::ostream& operator<< (std::ostream &out, const AnalysisStatistics& as) {
        out << "FPS: " << as.avgFPS << endl << "time: " << as.analysisDuration << " secs";
        return out;
    };

private:
    int videoFPS;
    int videoFramesLength;

    double avgFPS;
    clock_t analysisStart;
    double analysisDuration;

    // per second
    double counterFPS;
    double visibleFPS;



};


#endif //WUSHUVIDEOANALYZER_ANALYSISSTATISTICS_H
