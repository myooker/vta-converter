cmake_minimum_required(VERSION 3.31)
project(vidtoascii)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED core highgui imgcodecs videoio imgproc)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(vidtoascii main.cpp
        animation.cpp
        animation.h
        video.cpp
        video.h)
target_link_libraries(vidtoascii ${OpenCV_LIBS})
