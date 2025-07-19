#include <iostream>
#include <thread>
#include <csignal>
#include <sys/ioctl.h>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <ncurses.h>

#include "video.h"
#include "animation.h"

std::string getPath();

namespace vta {
    constexpr std::string_view palette{" .:-=+*#%@"}; // From lightest to darker
    // Hard-coded path to the picture.
    cv::Mat frame{
        cv::imread(
            "/home/myooker/Pictures/smart dude with glasses wallpaper.png",
            cv::IMREAD_REDUCED_GRAYSCALE_8)
    };

    winsize terminal{};

    // As character taller than a pixel, we use this constant to decrease video's height
    constexpr double ASCII_VERTICAL_PROP = 0.5;

    constexpr double REDUCE_SCALE_0 = 1.0;
    constexpr double REDUCE_SCALE_2 = 0.5;
    constexpr double REDUCE_SCALE_4 = 0.25;
    constexpr double REDUCE_SCALE_8 = 0.125;
}

struct VideoPar {
    double height{ 0.0 };
    double width{ 0.0 };

    void resize(const double REDUCE_SCALE) {
        height *= REDUCE_SCALE;
        width *= REDUCE_SCALE;
    }
};

enum CursorToggle {
    OFF = 0,
    ON = 1,
};

enum Constants {
    MAX_GRAY_VALUE = 255,
};

std::string getPath() {
    std::string path{};
    std::cout << "Please enter full path to a video\n> ";
    std::getline(std::cin, path);

    return path;
}

std::size_t getPaletteIndex(const unsigned int pixelColor) {
    return (pixelColor * (vta::palette.size()-1) / MAX_GRAY_VALUE);
}

std::vector<std::string> frameConvertToAscii(const cv::Mat& frame) {
    if (frame.empty()) {
        std::cerr << "Error! No image detected!\nExiting the program..." << std::endl;
        std::exit(1);
    }

    const std::size_t frameRowSize{ static_cast<std::size_t>( frame.rows )};    // Get frame's row f(x) size
    const std::size_t frameColSize{ static_cast<std::size_t>( frame.cols )};    // Get frame's column f(y) size
    std::vector<std::string> asciiFrame{}; /*
        ^           ^
    Represents  Frame's row
      frame
*/
    asciiFrame.resize(frameRowSize);    // Resizing std::string's in vector to frame's row f(x) size, without it the program crashes

    // At first, we're starting with a row
    for (std::size_t rowCount{}; rowCount < frameRowSize; rowCount++) {

        // Then, we're filling our row with columns, f(y) and print it to the terminal
        for (std::size_t colCount{}; colCount < frameColSize; colCount++) {
            unsigned int pixelValue{};
            pixelValue = frame.at<uchar>(static_cast<int>(rowCount), static_cast<int>(colCount));
            asciiFrame[rowCount].push_back( vta::palette[getPaletteIndex(pixelValue)] );
            std::cout << asciiFrame[rowCount][colCount];
        }
        // After filling the row, we go to the new line and filling the next row again
        std::cout << '\n';
    }

    return asciiFrame;
}

void ansiClearScreen() {
    std::cout << "\033[2J\033[H"; // Cleans terminal
}

void ansiMoveTopLeft() {
    std::cout << "\033[H"; // Moves terminal cursor to the top-left corner
}

void toggleCursor(const bool cursorState) {
    if (cursorState) {
        std::cout << "\033[?25h"; // Show cursor
    } else {
        std::cout << "\033[?25l"; // Hide cursor
    }
}

// This function is needed to restore cursor show state back at INTERCEPTION
void showCursor(int signal) {
    // ansiClearScreen();
    std::cout << "\033[?25h" << std::flush; // Show cursor

    std::exit(1);
}
// This function is needed to restore cursor show state back at std::atexit()
void showCursor() {
    // ansiClearScreen();
    std::cout << "\033[?25h" << std::flush; // Show cursor
}

bool isEnoughSpace(const winsize& terminalSize, VideoPar userVideo) {
    if (terminalSize.ws_col > userVideo.width && terminalSize.ws_row > userVideo.height)
        return true;
    else
        return false;
}

bool isEnoughSpace(const winsize& terminalSize, const cv::Mat& userVideo) {
    if (terminalSize.ws_col > userVideo.cols && terminalSize.ws_row > userVideo.rows)
        return true;
    else
        return false;
}

double autoSizeTerminal(const winsize& terminalSize, Video& userVideo) {
    auto userVidHeight = userVideo.getVideo().get(cv::CAP_PROP_FRAME_HEIGHT);
    auto userVidWidth = userVideo.getVideo().get(cv::CAP_PROP_FRAME_WIDTH);

    VideoPar userVideoPar{userVidHeight * vta::ASCII_VERTICAL_PROP, userVidWidth};

    // std::cout << "[BEFORE]\n" << "userVideoPar.height - " << userVideoPar.height
    //         << " userVideoPar.width - " << userVideoPar.width << '\n';

    double scale{ 1 };
    // while (true) {
    //     if (!isEnoughSpace(terminalSize, userVideoPar)) {
    //         userVideoPar.resize(0.5);
    //         scale *= 0.5;
    //         // std::cout << "[SCALING] ON" << '\n';
    //         continue;
    //     } else {
    //         break;
    //     }
    // }

    if (!isEnoughSpace(terminalSize, userVideoPar)) {
        userVideoPar.resize(0.5);
        scale *= 0.5;
        std::cout << "[SCALING] ON: " << scale << '\n';
        return scale;
    }

    // std::cout << "[AFTER]\n" << "userVideoPar.height - " << userVideoPar.height
    //     << " userVideoPar.width - " << userVideoPar.width << '\n';
    //
    // std::cout << "terminal size row : " << terminalSize.ws_row << '\n'
    //     << "terminal size col : " << terminalSize.ws_col << '\n';

    // std::system("clear");
}

void autoResizeFrame(cv::Mat& videoFrame) {
    winsize terminal{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal); // Get terminal size (rows and cols)

    if (!isEnoughSpace(terminal, videoFrame)) {
        while (true) {
            double SCALE_SIZE{ 1.0 };
            SCALE_SIZE *= 0.5;
            std::cout << "RESIZING...\n" << SCALE_SIZE << '\n';
        }
        cv::resize(videoFrame, videoFrame, cv::Size(), SCALE_SIZE, SCALE_SIZE * vta::ASCII_VERTICAL_PROP);
        // cv::resize(videoFrame, videoFrame, cv::Size(), vta::REDUCE_SCALE_8, vta::REDUCE_SCALE_8 * vta::ASCII_VERTICAL_PROP);
    }

    // cv::resize(videoFrame, videoFrame, cv::Size(), 1, 1 * vta::ASCII_VERTICAL_PROP);
}

int main() {
    // winsize terminal{};
    std::atexit(showCursor);
    std::signal(SIGINT, showCursor);
    // ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal); // Get terminal size (rows and cols)

    Video userVideo{ /*getPath()*/ };

    [[maybe_unused]] const double VIDEO_SCALE { /*autoSizeTerminal(terminal, userVideo)*/ };   // ...SCALE_0 - No scaling; SCALE_2 - reduce scale by 1/2; SCALE_4 - reduce scale by 1/4...
    using namespace std::chrono_literals;
    // std::cout << "VIDEO_SCALE : " << VIDEO_SCALE << '\n'
            // << "terminal size row : " << terminal.ws_row << '\n'
            // << "terminal size col : " << terminal.ws_col << '\n';

    std::this_thread::sleep_for(1s);
    // std::exit(0);

    // Animation asciiAnimation{};
    cv::Mat videoFrame{};
    userVideo.getVideo().read(videoFrame);
    cv::resize(videoFrame, videoFrame, cv::Size(), 1, 1 * vta::ASCII_VERTICAL_PROP);
    toggleCursor(OFF);
    ansiClearScreen();

    for (int frame{}; frame < static_cast<int>(userVideo.getFrameCount()); frame++) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(userVideo.getFpsDelay());          // Timer representing rate between frames, or just FPS
        userVideo.getVideo().read(videoFrame);     // Reads 1st frame and place it to the testFrame
        cv::cvtColor(videoFrame, videoFrame, cv::COLOR_RGB2GRAY);
        // cv::resize(videoFrame, videoFrame, cv::Size(), autoSizeTerminal(terminal, userVideo), autoSizeTerminal(terminal, userVideo) * vta::ASCII_VERTICAL_PROP);
        autoResizeFrame(videoFrame);
        frameConvertToAscii(videoFrame);
        ansiMoveTopLeft();
    }

    ansiClearScreen();
    toggleCursor(ON);
    std::cout << "FPS: " << userVideo.getFps() << '\n';
    std::cout << "Frame Count: " << userVideo.getFrameCount() << std::endl;

    return 0;
}
