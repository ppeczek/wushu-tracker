cmake_minimum_required(VERSION 3.7)
project(wushuVideoAnalyzer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp VideoAnalyzer.h Commons.cpp Commons.h AnalyzerSettings.cpp AnalyzerSettings.h Settings.cpp Settings.h CandidateContours.cpp CandidateContours.h AnalysisResultPoint.cpp AnalysisResultPoint.h Platform.cpp Platform.h Painter.cpp Painter.h ProcessedImage.cpp ProcessedImage.h KalmanEstimator.cpp KalmanEstimator.h AnalysisStatistics.cpp AnalysisStatistics.h)
add_executable(wushuVideoAnalyzer ${SOURCE_FILES})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(wushuVideoAnalyzer ${OpenCV_LIBS} ${Boost_LIBRARIES})