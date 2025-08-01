#include <iostream>
#include <thread>
#include <csignal>
#include <cstdlib>

#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>

#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>

#include <filesystem>

#include "video.h"
#include "utils/ansicode.h"

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
        PROGRAM = 0,
        COMMAND = 1,
        PARAMETER = 2,
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
    if (!path.starts_with('/') || path.starts_with("~/")) {
        errorType = Program::WRONG_PATH;
        return false;
    }

    if (!path.ends_with(".mp4")) {  // Add support for other formats that OpenCV supports
        errorType = Program::WRONG_FORM;
        return false;
    }

    if (!std::filesystem::exists(path)) {
        errorType = Program::NOT_EXIST;
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

void playCommand(const char **argv, ProgramSettings &currentSettings) {
    using namespace Program;

    if (argv[PARAMETER] == nullptr) {
        printError(NO_OPTION);
    }
    const std::string_view parameter{ argv[PARAMETER] };

    if (parameter == "help") {
        printError(NEGRO_ERROR);
    }

    if (argv[PATH] == nullptr) {
        printError(NO_PATH);
    }
    const std::string_view path{ argv[PATH] };

    if (parameter == "--file" || parameter == "-f") {
        if (FILE_ERROR ERROR_TYPE{}; isCorrectPath(path, ERROR_TYPE)) {
            currentSettings.videoPath = path;
        } else {
            printError(ERROR_TYPE);
        }
    }
}

void helpCommand(const char **argv) {

}

int main(const int argc, const char **argv) {
    std::atexit(ansi::showCursor);
    std::signal(SIGINT, ansi::showCursor);

    using namespace ftxui;
    using namespace std::chrono_literals;
    using namespace Program;
    ProgramSettings currentSettings{};

    for (std::size_t i{ 0 }; i < argc; i++) {
        if (argv[COMMAND] == nullptr) {
            helpCommand(argv);
            std::exit(1);
        }

        if (const std::string_view temp{ argv[i] }; temp == "play")
            playCommand(argv, currentSettings);

        if (const std::string_view temp{ argv[i]}; temp == "help")
            helpCommand(argv);
    }

    
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
