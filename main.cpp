//
// Created by ppeczek on 5/12/17.
//
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <aws/s3/model/Object.h>
#include <fstream>
//#include <pthread.h>

#include <boost/program_options.hpp>
#include "lib/VideoAnalyzer.cpp"

namespace po = boost::program_options;


void analyzeAll();

template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
    return os;
}

int main(int argc, char* argv[]) {
    // 1-1-1[1].mp4 --corner-AX=-90 --corner-AY=565 --corner-BX=788 --corner-BY=460 --corner-CX=1310 --corner-CY=480 --corner-DX=1010 --corner-DY=725
    // -std=c++11 `pkg-config opencv --cflags --libs`
    // https://s3.eu-central-1.amazonaws.com/wushu-videos/media/0001%5B1%5D.mp4
    Aws::String bucket_name = "wushu-videos";
    Aws::String key_name;

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    std::vector<std::string> inputFiles;
    Point cornerA = Commons::nullPoint,
          cornerB = Commons::nullPoint,
          cornerC = Commons::nullPoint,
          cornerD = Commons::nullPoint;

    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("version,v", "print version string")
                ("help", "produce help message")
                ("input-file", po::value<std::vector<std::string>>(), "input files")
                ("corner-AX", po::value<int>(&cornerA.x)->default_value(Commons::nullPoint.x), "cornerA.x")
                ("corner-AY", po::value<int>(&cornerA.y)->default_value(Commons::nullPoint.y), "cornerA.y")
                ("corner-BX", po::value<int>(&cornerB.x)->default_value(Commons::nullPoint.x), "cornerB.x")
                ("corner-BY", po::value<int>(&cornerB.y)->default_value(Commons::nullPoint.y), "cornerB.y")
                ("corner-CX", po::value<int>(&cornerC.x)->default_value(Commons::nullPoint.x), "cornerC.x")
                ("corner-CY", po::value<int>(&cornerC.y)->default_value(Commons::nullPoint.y), "cornerC.y")
                ("corner-DX", po::value<int>(&cornerD.x)->default_value(Commons::nullPoint.x), "cornerD.x")
                ("corner-DY", po::value<int>(&cornerD.y)->default_value(Commons::nullPoint.y), "cornerD.y")
                ;

        po::positional_options_description p;
        p.add("input-file", -1);

        po::variables_map vm;
        store(po::command_line_parser(argc, argv).
                options(desc).positional(p).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }

        if (vm.count("version")) {
            cout << "Wushu video analyzer, version " << Settings::version << "\n";
            return 0;
        }

        if (vm.count("input-file")) {
            inputFiles = vm["input-file"].as<std::vector<std::string>>();
            cout << "Input files are: " << inputFiles << "\n";
        }

//        for (auto& file: inputFiles) {
            Aws::Client::ClientConfiguration config;
            config.region = "eu-central-1";
            Aws::S3::S3Client s3_client(config);

            Aws::S3::Model::ListObjectsRequest objects_request;
            objects_request.WithBucket(bucket_name);

            auto list_objects_outcome = s3_client.ListObjects(objects_request);

            if (list_objects_outcome.IsSuccess())
            {
                Aws::Vector<Aws::S3::Model::Object> object_list =
                        list_objects_outcome.GetResult().GetContents();

                for (auto const &s3_object : object_list)
                {
                    std::cout << "* " << s3_object.GetKey() << std::endl;
                }
            }
            else
            {
                std::cout << "ListObjects error: " <<
                          list_objects_outcome.GetError().GetExceptionName() << " " <<
                          list_objects_outcome.GetError().GetMessage() << std::endl;
            }
//            key_name = "media/PAWC-1.mp4";

//            Aws::S3::Model::GetObjectRequest object_request;
//            object_request.WithBucket(bucket_name).WithKey(key_name);
//
//            auto get_object_outcome = s3_client.GetObject(object_request);
//
//            if (get_object_outcome.IsSuccess()) {
//                Aws::OFStream local_file;
//                local_file.open(key_name.c_str(), std::ios::out | std::ios::binary);
//                local_file << get_object_outcome.GetResult().GetBody().rdbuf();
//                std::cout << "Done!" << std::endl;
//            } else {
//                std::cout << "GetObject error: " <<
//                          get_object_outcome.GetError().GetExceptionName() << " " <<
//                          get_object_outcome.GetError().GetMessage() << std::endl;
//            }
//        }

//        clock_t start = clock();
//        std::vector<VideoAnalyzer> analyzers;
//
//        for (auto& file: inputFiles) {
//            analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings(file), {cornerA, cornerB, cornerC, cornerD}));
//        }
//
//        int i = 0;
//        for (auto&& va : analyzers) {
//            cout << "Video " << ++i << endl;
//            va.analyze(va.PPECZEK);
////        va.analyze(va.MIL);
////        va.analyze(va.BOOSTING);
////        va.analyze(va.KCF);
////        va.analyze(va.TLD);
////        va.analyze(va.MEDIANFLOW);
////        va.analyze(va.GOTURN);
//            cout << va.getStatistics() << endl;
//            cout << endl;
//
//            va.drawPaths();
//        }
//
//        double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
//        cout << "Program duration: " << duration << endl;

    }
    catch(exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }

    Aws::ShutdownAPI(options);
    return 0;
}

void analyzeAll() {
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

    clock_t start = clock();
    std::vector<VideoAnalyzer> analyzers;

    // corners order: top-left, top-right, bottom-right, bottom-left

    // 12th WWC videos #1
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[1]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[2]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[3]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[4]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[5]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-1[6]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));

    // 12th WWC videos #2
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[1]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[2]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[3]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[4]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[5]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[6]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-2[7]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));

    // 12th WWC videos #3
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[1]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[2]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[3]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[4]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[5]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[6]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[7]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("1-1-3[8]"), {Point(-90, 565), Point(788, 460), Point(1310, 480), Point(1010, 725)}));

    // 12th WWC videos #4
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[1]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[2]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[3]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[4]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[5]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[6]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0001[7]"), {Point(-90, 510), Point(788, 390), Point(1310, 400), Point(960, 650)}));

    // 12th WWC videos #5
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[1]"), {Point(920, 420), Point(1450, 645), Point(-460, 625), Point(390, 420)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[2]"), {Point(920, 420), Point(1450, 645), Point(-460, 625), Point(390, 420)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[3]"), {Point(920, 420), Point(1450, 645), Point(-460, 625), Point(390, 420)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[4]"), {Point(955, 425), Point(1580, 700), Point(-570, 670), Point(390, 425)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("0005[5]"), {Point(955, 425), Point(1580, 700), Point(-570, 670), Point(390, 425)}));

    // PAWC videos
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-1"), {Point(0, 505), Point(880, 465), Point(1260, 570), Point(140, 700)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-2"), {Point(10, 498), Point(880, 462), Point(1260, 565), Point(150, 695)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-3"), {Point(17, 488), Point(880, 458), Point(1260, 563), Point(155, 680)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-4"), {Point(17, 488), Point(880, 458), Point(1260, 563), Point(160, 685)}));
    analyzers.emplace_back(VideoAnalyzer(AnalyzerSettings("PAWC-5"), {Point(8, 455), Point(875, 420), Point(1255, 525), Point(145, 650)}));

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
}