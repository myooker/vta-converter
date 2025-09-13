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
    struct cliArgs {
        std::string_view command{};
        std::string_view path{};
    };

    struct Settings {
        std::string videoPath{"None"};
        bool isHelp{ false };
    };

    enum FILE_ERROR {
        NOT_EXIST,
        WRONG_FORM,
        WRONG_PATH,
        NO_OPTION,
        NO_PATH,
        UNK_COMMAND,
    };

    enum ARG_INDEX {
        PROGRAM = 0,
        COMMAND = 1,
        PATH = 2,
    };

    namespace State {
        cliArgs Args{};
        Settings Settings{};
    }

    constexpr std::array supportedFormats{ ".avi", ".mp4", ".mov", ".mkv", ".flv", ".wmv", ".3gp" };
}

using namespace Program;

bool isFileExist(const std::string_view path) {
    struct stat buffer{};
    const std::string temp{ path };
    return stat(temp.c_str(), &buffer) == 0;
}

bool isCorrectPath(const std::string_view path, FILE_ERROR &errorType) {
    if (!path.starts_with('/') && !path.starts_with("~/")) {
        errorType = WRONG_PATH;
        return false;
    }
    //TODO: Add other formats that OpenCV can work with
    if (!path.ends_with(".mp4")) {
        errorType = WRONG_FORM;
        return false;
    }
    if (!isFileExist(path)) {
        errorType = NOT_EXIST;
        return false;
    }

    return true;
}

void printError(const FILE_ERROR ERROR_TYPE) {
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
                    << "hint: use 'play <PATH>' to specify a video file\n\n";
            break;
        case NO_OPTION:
            ;
        case UNK_COMMAND:
            std::cout << "error: unknown command '" << State::Args.command << "'\n"
                    << "hint: use 'help' to see available commands\n\n";
        default:;
    }
    std::cout << "Make sure to:\n"
        << "- Use absolute path to an existing file\n"
        << "- Avoid using unsupported character or symbols\n\n"
        << "Example:\n"
        << "\t./vta play /home/username/video/sample.mp4\n"
        << "\t./vta play ~/video/sample.mp4" << std::endl;

    std::exit(1);
}

void parseCliArguments(const char **argv, const int argc) {
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
                if (argc < 3)
                    printError(NO_PATH);

                State::Args.command = tempArg;
                continue;
            }
            if (tempArg == "help") {
                State::Args.command = tempArg;
                continue;
            }
            State::Args.command = tempArg;
            printError(UNK_COMMAND);
        }

        if (i == PATH) {
            if (isCorrectPath(tempArg, ERROR_TYPE)) {
                State::Args.path = tempArg;
            } else {
                printError(ERROR_TYPE);
            }
        }
    }
}

void playVideo() {
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
            << "   ./vta help\t\tShow this help message.\n\n"
            << "Commands:\n"
            << "   play\t\t\tPlay a file. <PATH> must be a valid file path.\n"
            << "   help\t\t\tDisplay this help message and exit.\n\n"
            << "Examples:\n"
            << "   ./vta play /home/user/video.mp4\n"
            << "   ./vta help\n";
    std::exit(0);
}

void initFromArgs() {
    if (State::Args.command == "play")
        playVideo();
    else
        printHelp();
}

int main(const int argc, const char **argv) {
    std::atexit(ansi::showCursor);
    std::signal(SIGINT, ansi::showCursor);

    using namespace ftxui;
    using namespace std::chrono_literals;

    parseCliArguments(argv, argc);
    State::Settings.videoPath = State::Args.path;

    initFromArgs();

    return 0;
}