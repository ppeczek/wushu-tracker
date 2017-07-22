cmake_minimum_required(VERSION 3.7)
project(wushuVideoAnalyzer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Commons.cpp Commons.h AnalyzerSettings.cpp AnalyzerSettings.h Settings.cpp Settings.h)

find_package(OpenCV REQUIRED)
add_executable(wushuVideoAnalyzer ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(wushuVideoAnalyzer ${OpenCV_LIBS})
