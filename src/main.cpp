// C++ STD library
#include <iostream>
#include <thread>
#include <csignal>
#include <cstdlib>

// OpenCV library
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>

// ftxui library
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>

// My shit
#include "video.h"
#include "animation.h"
#include "engine/constants.h"
#include "engine/scaling.h"
#include "utils/ansicode.h"
#include "utils/debug_func.h"

std::string getPath();

enum CursorToggle {
    OFF = 0,
    ON = 1,
};

std::string getPath() {
    std::string path{};
    std::cout << "Please enter full path to a video\n> ";
    std::getline(std::cin, path);

    if (!path.starts_with('/')) {
        std::cerr << "Invalid path! Please try again!\n";
        std::exit(1);
    }

    if (path.starts_with('~')) {
        path.replace(0, 1, std::getenv("HOME"));
    }

    return path;
}

int main(int argc, char** argv) {
    using namespace ftxui;
    using namespace std::chrono_literals;

    bool beVerbose{ false };

    std::atexit(ansi::showCursor);
    std::signal(SIGINT, ansi::showCursor);
    std::string tempPath{};

    for (int i{}; i < argc; i++) {
        if (std::string(argv[i]) == "-v") {
            beVerbose = true;
            continue;
        }
        if (std::string(argv[i]) == "-f" && argc > 2) { // It's shit and will brake
            tempPath = argv[static_cast<std::size_t>(i+1)];
            continue;
        }
    }

    if (tempPath.empty()) {
        std::cerr << "Please enter video's path! For example:\n"
                << "1) /home/myooker/video.mp4\n"
                << "2) ~/video.mp4\n"
                << "Exiting the program..." << std::endl;
        std::exit(1);
    }

    Video userVideo{tempPath};
    cv::Mat videoFrame{};

    if (beVerbose) {
        DEBUG::VIDEO::printFpsDelay(userVideo);
        DEBUG::VIDEO::printFrameCount(userVideo);
        DEBUG::VIDEO::printFrameRate(userVideo);

        std::this_thread::sleep_for(5s);
    }

    ansi::toggleCursor(OFF);
    ansi::clearScreen();

    userVideo.playAscii(videoFrame);

    ansi::clearScreen();

    return 0;
}
