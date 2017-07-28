//
// Created by ppeczek on 7/21/17.
//

#ifndef WUSHUVIDEOANALYZER_ANALYZERSETTINGS_H
#define WUSHUVIDEOANALYZER_ANALYZERSETTINGS_H

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

#include "Settings.h"
#include "Commons.h"


class AnalyzerSettings {
public:
    // paths
    String dir;
    String allResultsPath;
    String samplePath;
    String framesPath;
    String resultsPath;
    String imagesPath;

    String videoPath;
    String patternPath;

    String localizationsPath;
    String pathsImagePath;

    String localizationsKalmanPath;
    String pathsImageKalmanPath;

    const int nFrames = 10;
    const int imageWidth = 700;

    bool debug = true;
    bool applyKalman = true;
    bool showDetailText = false;

    bool doResize;
    float resizeFactor;

    int thresholdFactor;
    int blurFactor;

    struct stat info;

    explicit AnalyzerSettings(const String _dir) : dir(_dir) {
        // params
        thresholdFactor = 15;
        blurFactor = 5;

        // paths
        samplePath = "media/";
        allResultsPath = "results/";
        resultsPath = allResultsPath + dir + "/";
        framesPath = resultsPath + "frames/";
        imagesPath = resultsPath + "result-imgs/";

        videoPath = getFilename(samplePath, dir, Settings::videoExt);
        patternPath = getFilename(resultsPath, Settings::patternName, Settings::imgExt);

        localizationsPath = getFilename(resultsPath, Settings::resultsName, Settings::resultsExt);
        pathsImagePath = getFilename(imagesPath, Settings::pathsName, Settings::imgExt);

        localizationsKalmanPath = getFilename(resultsPath, Settings::resultsName + "-" + Settings::kalmanName, Settings::resultsExt);
        pathsImageKalmanPath = getFilename(imagesPath, Settings::pathsName + "-" + Settings::kalmanName, Settings::imgExt);

        for (auto&& path: {allResultsPath, resultsPath, framesPath, imagesPath}) {
            if (stat(path.c_str(), &info) != 0) {
                mkdir(path.c_str(), ACCESSPERMS);
            }
        }
    };

    void adjustResizeImageParameters(Mat img) {
        resizeFactor = (float)imageWidth / (float)img.cols;
        doResize = resizeFactor != 1;
    }

private:
    String getFilename(const String path, const String name, const String ext) {
        return path + name + "." + ext;
    };

};


#endif //WUSHUVIDEOANALYZER_ANALYZERSETTINGS_H
