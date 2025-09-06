#include <iostream>
#include <thread>
#include <csignal>
#include <cstdlib>
#include <opencv4/opencv2/videoio.hpp>
#include <ftxui/screen/screen.hpp>
#include <filesystem>
#include "video.h"
#include "utils/ansicode.h"
#include "sys/stat.h"

namespace Program{
    namespace Parameter {
        constexpr std::array<std::string_view, 2> file {"--file", "-f"};
        constexpr std::array<std::string_view, 2> cache {"--cache", "-c"};
    }

    struct cliArgs {
        std::string_view command{};
        std::string_view parameter{};
        std::string_view path{};
    };

    struct Settings {
        std::string videoPath{"None"};
        bool isVerbose{ false };
        bool isHelp{ false };
    };

    enum FILE_ERROR {
        NEGRO_ERROR,
        NOT_EXIST,
        WRONG_FORM,
        WRONG_PATH,
        NO_OPTION,
        NO_PATH,
    };

    enum ARG_INDEX {
        PROGRAM = 0,
        COMMAND = 1,
        PARAMETER = 2,
        PATH = 3,
    };

    namespace State {
        cliArgs Args{};
        Settings Settings{};
    }

}

bool isFileExist(const std::string_view path) {
    struct stat buffer;
    std::string temp{ path };
    return stat(temp.c_str(), &buffer) == 0;
}

bool isCorrectPath(const std::string_view path, Program::FILE_ERROR &errorType) {
    using namespace Program;
    if (!path.starts_with('/') && !path.starts_with("~/")) {
        errorType = WRONG_PATH;
        return false;
    }

    if (!path.ends_with(".mp4")) {                  // Add support for other formats that OpenCV supports
        errorType = WRONG_FORM;
        return false;
    }

    if (!isFileExist(path)) {
        errorType = NOT_EXIST;
        return false;
    }

    return true;
}

void printError(const Program::FILE_ERROR ERROR_TYPE) {
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
        default:;
    }

    std::cout << "Make sure to:\n"
        << "- Use absolute path to an existing file\n"
        << "- Avoid using unsupported character or symbols\n\n"
        << "Example:\n"
        << "\t/home/username/video/sample.mp4\n"
        << "\t~/video/sample.mp4" << std::endl;

    std::exit(1);
}

void parseCliArguments(const char **argv, const int argc) {
    using namespace Program;
    using namespace std::ranges;

    FILE_ERROR ERROR_TYPE;

    for (std::size_t i{1}; i < argc; i++) {
        if (argv[i] == nullptr) {
            std::cout << "nullptr!!!\n";
            std::exit(-1);
        }

        const std::string_view tempArg{ argv[i] };

        if (i == COMMAND) {
            if (tempArg == "play") {
                State::Args.command = tempArg;
                continue;
            }

            if (tempArg == "help") {
                State::Args.command = tempArg;
                continue;
            }
        }

        if (i == PARAMETER) {
            if (const auto it = find(Parameter::file, tempArg); it != Parameter::file.end()) {
                State::Args.parameter = tempArg;
                continue;
            }

            if (const auto it = find(Parameter::cache, tempArg); it != Parameter::cache.end()) {
                State::Args.parameter = tempArg;
                continue;
            }
        }

        if (i == PATH) {
            if (isCorrectPath(tempArg, ERROR_TYPE)) {
                State::Args.path = tempArg;
                continue;
            } else {
                printError(ERROR_TYPE);
            }
        }
    }
}

void playVideo() {
    using namespace Program;
    Video userVideo{ State::Settings.videoPath };
    cv::Mat videoFrame{};

    ansi::toggleCursor(ansi::CursorToggle::OFF);
    ansi::clearScreen();
    userVideo.playAscii(videoFrame);
    ansi::clearScreen();
}

void printHelp() {
    std::cout << "Usage:\n"
            << "   ./vta play <PATH>\tPlay the specified file or media from the given path.\n"
            << "   ./vta help\t Show this help message.\n\n"
            << "Commands:\n"
            << "   play\tPlay a file. <PATH> must be a valid file path.\n"
            << "   help\tDisplay this help message and exit.\n\n"
            << "Examples:\n"
            << "   ./vta play /home/user/video.mp4\n"
            << "   ./vta help\n";
    std::exit(0);
}

void initFromArgs() {
    using namespace Program;

    if (State::Args.command == "play") {
        playVideo();
    } else {
        printHelp();
    }
}

int main(const int argc, const char **argv) {
    std::atexit(ansi::showCursor);
    std::signal(SIGINT, ansi::showCursor);

    using namespace ftxui;
    using namespace std::chrono_literals;
    using namespace Program;

    parseCliArguments(argv, argc);
    State::Settings.videoPath = State::Args.path;

    initFromArgs();

    return 0;
}