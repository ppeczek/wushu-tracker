cmake_minimum_required(VERSION 3.7)
project(wushuVideoAnalyzer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp src/VideoAnalyzer.h src/Commons.cpp src/Commons.h src/AnalyzerSettings.cpp src/AnalyzerSettings.h src/Settings.cpp src/Settings.h src/CandidateContours.cpp src/CandidateContours.h src/AnalysisResultPoint.cpp src/AnalysisResultPoint.h src/Platform.cpp src/Platform.h src/Painter.cpp src/Painter.h src/ProcessedImage.cpp src/ProcessedImage.h src/KalmanEstimator.cpp src/KalmanEstimator.h src/AnalysisStatistics.cpp src/AnalysisStatistics.h)
add_executable(wushuVideoAnalyzer ${SOURCE_FILES})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(AWSSDK REQUIRED)
add_definitions(-DUSE_IMPORT_EXPORT)

target_link_libraries(wushuVideoAnalyzer ${OpenCV_LIBS} ${Boost_LIBRARIES} aws-cpp-sdk-s3 aws-cpp-sdk-core)