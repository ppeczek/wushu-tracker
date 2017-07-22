//
// Created by ppeczek on 7/21/17.
//

#ifndef WUSHUVIDEOANALYZER_ANALYZERSETTINGS_H
#define WUSHUVIDEOANALYZER_ANALYZERSETTINGS_H

#include <iostream>

#include "Settings.h"


class AnalyzerSettings {
public:
    // paths
    String dir;
    String samplePath;
    String framesPath;
    String resultsPath;

    String videoPath;
    String patternPath;

    String localizationsPath;
    String pointsImagePath;
    String pathsImagePath;

    String localizationsMinPath;
    String pointsMinImagePath;
    String pathsMinImagePath;

    String estimatedLocalizationsPath;
    String estimatedPointsImagePath;
    String estimatedPathsImagePath;

    const int nFrames = 10;

    explicit AnalyzerSettings(const String _dir) : dir(_dir) {
        samplePath = "media/" + dir + "/";
        framesPath = samplePath + "frames/";
        resultsPath = samplePath + "results/";

        videoPath = getFilename(samplePath, Settings::videoName, Settings::videoExt);
        patternPath = getFilename(samplePath, Settings::patternName, Settings::imgExt);

        localizationsPath = getFilename(resultsPath, Settings::resultsName, Settings::resultsExt);
        pointsImagePath = getFilename(resultsPath, Settings::pointsName, Settings::imgExt);
        pathsImagePath = getFilename(resultsPath, Settings::pathsName, Settings::imgExt);

        localizationsMinPath = getFilename(resultsPath, Settings::resultsName + "-" + Settings::minifyName, Settings::resultsExt);
        pointsMinImagePath = getFilename(resultsPath, Settings::pointsName + "-" + Settings::minifyName, Settings::imgExt);
        pathsMinImagePath = getFilename(resultsPath, Settings::pathsName + "-" + Settings::minifyName, Settings::imgExt);

        estimatedLocalizationsPath = getFilename(resultsPath, Settings::resultsName + "-" + Settings::kalmanName, Settings::resultsExt);
        estimatedPointsImagePath = getFilename(resultsPath, Settings::pointsName + "-" + Settings::kalmanName, Settings::imgExt);
        estimatedPathsImagePath = getFilename(resultsPath, Settings::pathsName + "-" + Settings::kalmanName, Settings::imgExt);
    };

    String getFilename(const String path, const String name, const String ext) {
        return path + name + "." + ext;
    };

};


#endif //WUSHUVIDEOANALYZER_ANALYZERSETTINGS_H
