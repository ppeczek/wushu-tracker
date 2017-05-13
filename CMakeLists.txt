cmake_minimum_required(VERSION 3.7)
project(wushuVideoAnalyzer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

find_package(OpenCV REQUIRED)
add_executable(medianFilter ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(medianFilter ${OpenCV_LIBS})
