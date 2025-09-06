cmake_minimum_required(VERSION 3.31)
project(vidtoascii)

set(CMAKE_CXX_STANDARD 20)

# --- Fetch FTXUI ---
include(FetchContent)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# --- Find OpenCV ---
find_package(OpenCV REQUIRED core highgui imgcodecs videoio imgproc)
include_directories(${OpenCV_INCLUDE_DIRS})

# --- Define executable ---
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
        engine/constants.cpp
        engine/constants.h
)

# --- Link libraries ---
target_link_libraries(vidtoascii
        PRIVATE
        ftxui::dom
        ftxui::screen
        ${OpenCV_LIBS}
)

# --- Set output binary name ---
set_target_properties(vidtoascii PROPERTIES OUTPUT_NAME "vta")

# --- Install rules ---
# Install the executable
install(TARGETS vidtoascii
        RUNTIME DESTINATION bin
)

# Install headers (optional if you want them usable by others)
install(FILES
        video.h
        utils/debug_func.h
        utils/ansicode.h
        engine/scaling.h
        engine/constants.h
        DESTINATION include/vidtoascii
)
