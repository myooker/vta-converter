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
#include "utils/ansicode.h"
#include "utils/debug_func.h"

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

struct ProgramSettings {
    std::string videoPath{"None"};
    bool isVerbose{ false };
    bool isHelp{ false };

};

bool isCorrectPath(const std::string& path) {
    if (path.starts_with('/') || path.starts_with("~/")) {
        return true;
    } else {
        return false;
    }
}

void invalidFilePathMsg() {
    std::cout << "error: invalid file path\n\n"
        << "Make sure to:\n"
        << "- Use absolute path to an existing file\n"
        << "- Avoid using unsupported character or symbols\n\n"
        << "Example:\n"
        << "\t/home/username/video/sample.mp4\n"
        << "\t~/video/sample.mp4" << std::endl;
}

int main(int argc, char** argv) {
    using namespace ftxui;
    using namespace std::chrono_literals;
    ProgramSettings currentSettings{};

    std::atexit(ansi::showCursor);
    std::signal(SIGINT, ansi::showCursor);

    std::cout << argc << '\n';

    for (std::size_t x{ 1 }; x < argc; ++x) {
        std::cout << x << ") argv: " << argv[x] << '\n';

        if (std::string tempArg{argv[x]}; tempArg.starts_with("--")) {
            if (tempArg.substr(2) == "verbose") {
                currentSettings.isVerbose = true;
            }

            if (tempArg.substr(2) == "file") {
                if (!isCorrectPath(argv[x+1])) {
                    invalidFilePathMsg();
                    return 1;
                }
                currentSettings.videoPath = argv[x+1];
                break;
            }

            if (tempArg.substr(2) == "help" && argc < 3) {
                //printhelp();
                std::cout << "HELP!\n";
                return 0;
            } else {
                std::cerr << "Use '--help' or '-h' alone to display usage information.\n";
                return 1;
            }
        }

        if (std::string tempArg{argv[x]}; tempArg.starts_with('-')) {
            if (tempArg.find('v')) {
                currentSettings.isVerbose = true;
            }

            if (tempArg.find('f')) {
                if (!isCorrectPath(argv[x+1])) {
                    invalidFilePathMsg();
                    return 1;
                }
                currentSettings.videoPath = argv[x+1];
                break;
            }

            if (tempArg.find('h') && argc < 3) {
                //printhelp();
                std::cout << "HELP!\n";
                return 0;
            } else {
                std::cerr << "Use '--help' or '-h' alone to display usage information.\n";
                return 1;
            }
        }
    }

    std::cout << "isVerbose: " << currentSettings.isVerbose << '\n'
                << "isHelp: " << currentSettings.isHelp << '\n'
                << "filePath: " << currentSettings.videoPath << '\n';


    return 0;

    // for (int i{ 1 }; i < argc; i++) {
    //     if (std::string(argv[i]) == "-v") {
    //         beVerbose = true;
    //         continue;
    //     }
    //     if (std::string(argv[i]) == "-f" && argv[i+1]) { // It's shit and will brake
    //         videoPath = argv[static_cast<std::size_t>(i+1)];
    //         continue;
    //     }
    //
    //     if (std::string(argv[i]) == "-h") {
    //         std::cout << "Help Information:\n";
    //         return 0;
    //     }
    // }

    // if (videoPath.empty()) {
    //     std::cerr << "Please enter video's path! For example:\n"
    //             << "1) /home/myooker/video.mp4\n"
    //             << "2) ~/video.mp4\n"
    //             << "Exiting the program..." << std::endl;
    //     return 1;
    // }

    Video userVideo{ currentSettings.videoPath };
    cv::Mat videoFrame{};

    // if (beVerbose) {
    //     DEBUG::APP::printFilePath(videoPath);
    //     DEBUG::VIDEO::printRepeat(false);
    //     DEBUG::VIDEO::printFpsDelay(userVideo);
    //     DEBUG::VIDEO::printFrameCount(userVideo);
    //     DEBUG::VIDEO::printFrameRate(userVideo);
    //     DEBUG::APP::printSleepTime();
    //     std::this_thread::sleep_for(5s);
    // }

    ansi::toggleCursor(OFF);
    ansi::clearScreen();

    userVideo.playAscii(videoFrame);

    ansi::clearScreen();

    return 0;
}
