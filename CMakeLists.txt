cmake_minimum_required(VERSION 3.28)
project(vidtoascii VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
        ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

find_package(OpenCV REQUIRED core highgui imgcodecs videoio imgproc)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
)

add_executable(vta
        main.cpp
        video.cpp
        utils/debug_func.cpp
        utils/ansicode.cpp
        engine/scaling.cpp
        engine/constants.cpp
)

target_link_libraries(vta
        PRIVATE
        ftxui::dom
        ftxui::screen
        ${OpenCV_LIBRARIES}
)

install(TARGETS vta
        RUNTIME DESTINATION bin
)

install(FILES
        video.h
        utils/debug_func.h
        utils/ansicode.h
        engine/scaling.h
        engine/constants.h
        DESTINATION include/vidtoascii
)