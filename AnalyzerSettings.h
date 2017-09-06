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
    std::string dir;
    std::string allResultsPath;
    std::string samplePath;
    std::string framesPath;
    std::string resultsPath;
    std::string imagesPath;

    std::string videoPath;
    std::string patternPath;

    std::string localizationsPath;
    std::string pathsImagePath;

    std::string localizationsKalmanPath;
    std::string pathsImageKalmanPath;

    bool doResize;
    float resizeFactor;

    struct stat info;

    explicit AnalyzerSettings(const std::string _dir) : dir(_dir) {
        // paths
        samplePath = "media/";
        allResultsPath = "results/";
        resultsPath = allResultsPath + dir + "/";
        framesPath = resultsPath + "frames/";
        imagesPath = resultsPath + "result-imgs/";

        videoPath = samplePath + dir;
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

    void adjustResizeImageParameters(cv::Mat img) {
        resizeFactor = (float)Settings::imageWidth / (float)img.cols;
        doResize = resizeFactor != 1;
    }

private:
    std::string getFilename(const std::string path, const std::string name, const std::string ext) {
        return path + name + "." + ext;
    };

};


#endif //WUSHUVIDEOANALYZER_ANALYZERSETTINGS_H
