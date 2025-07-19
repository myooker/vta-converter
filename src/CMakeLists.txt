cmake_minimum_required(VERSION 3.31)
project(vidtoascii)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

find_package(OpenCV REQUIRED core highgui imgcodecs videoio imgproc)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(vidtoascii
        main.cpp
        animation.cpp
        animation.h
        video.cpp
        video.h
        ../debug_func.cpp
)

target_link_libraries(vidtoascii
        PRIVATE
        ftxui::dom
        ftxui::screen
        ${OpenCV_LIBS}
)
