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
        video.cpp
        video.h
        utils/debug_func.cpp
        utils/debug_func.h
        utils/ansicode.cpp
        utils/ansicode.h
        engine/scaling.cpp
        engine/scaling.h
        engine/constants.h
        engine/constants.cpp
)

target_link_libraries(vidtoascii
        PRIVATE
        ftxui::dom
        ftxui::screen
        ${OpenCV_LIBS}
)

set_target_properties(vidtoascii PROPERTIES OUTPUT_NAME "vta")