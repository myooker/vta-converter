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
#include <filesystem>

#include "video.h"
#include "utils/ansicode.h"
#include "utils/debug_func.h"

namespace Program{
    constexpr std::string_view commandPlay{"play"};
    constexpr std::string_view commandHelp{"help"};

    enum FILE_ERROR {
        NEGRO_ERROR,
        NOT_EXIST,

        WRONG_FORM,
        WRONG_PATH,

        NO_OPTION,
        NO_PATH,
    };

    enum ARG_INDEX {
        OPTION = 2,
        PATH = 3,

    };
}

enum CursorToggle {
    OFF = 0,
    ON = 1,
};

struct ProgramSettings {
    std::string videoPath{"None"};
    bool isVerbose{ false };
    bool isHelp{ false };
};

bool isCorrectPath(const std::string_view& path, Program::FILE_ERROR &errorType) {
    int c{};

    if (path.starts_with('/') || path.starts_with("~/")) {
        c += 1;
    } else {
        errorType = Program::WRONG_PATH;
        return false;
    }

    if (path.ends_with(".mp4")) {
        c += 1;
    } else {
        errorType = Program::WRONG_FORM;
        return false;
    }

    if (std::filesystem::exists(path)) {
        c += 1;
    } else {
        errorType = Program::NOT_EXIST;
        return false;
    }

    if (c > 2)
        return true;
    else
        return false;
}

void invalidFile(const Program::FILE_ERROR ERROR_TYPE) {
    using namespace Program;
    switch (ERROR_TYPE) {
        case WRONG_FORM:
            std::cout << "error: incorrect video format\n\n";
            break;
        case WRONG_PATH:
            std::cout << "error: invalid file path\n\n";
            break;
        case NOT_EXIST:
            std::cout << "error: the file does not exist\n\n";
            break;
        case NO_PATH:
            std::cout << "error: no path provided\n"
                    << "hint: use --file <PATH> to specify a video file\n\n";
            break;
        case NO_OPTION:
            std::cout << "error: missing required option\n"
                     << "hint: expected --file <PATH> or --cache <PATH> after the command\n\n";
        default: ;
    }

    std::cout << "Make sure to:\n"
    << "- Use absolute path to an existing file\n"
    << "- Avoid using unsupported character or symbols\n\n"
    << "Example:\n"
    << "\t/home/username/video/sample.mp4\n"
    << "\t~/video/sample.mp4" << std::endl;

    std::exit(1);
}

void playCommand(const char **argv, ProgramSettings &currentSettings) {
    using namespace Program;
    if (argv[OPTION] == nullptr) {
        invalidFile(NO_OPTION);
        return;
    }

    if (argv[PATH] == nullptr) {
        invalidFile(NO_PATH);
        return;
    }

    FILE_ERROR ERROR_TYPE{};

    const std::string_view argument{ argv[2] };
    const std::string_view path{ argv[3] };

    if (argument == "--file" || argument == "-f") {
        if (isCorrectPath(path, ERROR_TYPE)) {
            currentSettings.videoPath = path;
        } else {
            invalidFile(ERROR_TYPE);
        }
    }
}

int main(const int argc, const char **argv) {
    std::atexit(ansi::showCursor);
    std::signal(SIGINT, ansi::showCursor);

    using namespace ftxui;
    using namespace std::chrono_literals;
    ProgramSettings currentSettings{};
    playCommand(argv, currentSettings);
    
    // for (int i{ 1 }; i < argc; i++) {
    //     if (std::string_view{ argv[i] } == Program::commandPlay && argc > 3) {
    //
    //         if (const std::string_view fileArg{argv[i+1]}; fileArg == "-f" || fileArg == "--file") {
    //             if (Program::FILE_ERROR ERROR_TYPE{}; isCorrectPath(std::string_view{argv[i+2]}, ERROR_TYPE)) {
    //                 currentSettings.videoPath = argv[i+2];
    //             } else {
    //                 invalidFile(ERROR_TYPE);
    //                 return 1;
    //             }
    //         } else {
    //
    //         }
    //         if (const std::string_view cacheArg{argv[i+1]}; cacheArg == "-c" || cacheArg == "--cache") {
    //
    //         }
    //     }
    //
    //     if (std::string_view {argv[i]} == Program::commandHelp) {
    //         //printHelp();
    //         return 0;
    //     }
    // }

    std::cout << "isVerbose: " << currentSettings.isVerbose << '\n'
                << "isHelp: " << currentSettings.isHelp << '\n'
                << "filePath: " << currentSettings.videoPath << '\n';

    Video userVideo{ currentSettings.videoPath };
    cv::Mat videoFrame{};

    ansi::toggleCursor(OFF);
    ansi::clearScreen();

    userVideo.playAscii(videoFrame);

    ansi::clearScreen();

    return 0;
}
