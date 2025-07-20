#include <iostream>
#include <thread>
#include <csignal>
#include <sys/ioctl.h>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>

#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>

#include "video.h"
#include "animation.h"
#include "utils/debug_func.h"

std::string getPath();

namespace vta {
    constexpr std::string_view palette{" .:-=+*#%@"};       // From lightest to darker
    constexpr std::string_view dpalette{"@%#*+=-:. "};      // From darker to lightes

    Video userVideo{ /*getPath()*/ };

    // As character taller than a pixel, we use this constant to decrease video's height
    constexpr double ASCII_VERTICAL_SCALE = 0.5;
}

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
        }
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
    ansiClearScreen();
    std::cout << "\033[?25h" << std::flush; // Show cursor

    std::exit(1);
}
// This function is needed to restore cursor show state back at std::atexit()
void showCursor() {
    ansiClearScreen();
    std::cout << "\033[?25h" << std::flush; // Show cursor
}

double scalingFactor(const cv::Mat& videoFrame, const ftxui::Dimensions& screen) {
    const auto termWidth  = static_cast<double>(screen.dimx);
    const auto termHeight = static_cast<double>(screen.dimy);

    const auto frameWidth  = static_cast<double>(videoFrame.cols);
    const auto frameHeight = static_cast<double>(videoFrame.rows) * vta::ASCII_VERTICAL_SCALE;

    const double scaleX = termWidth / frameWidth;
    const double scaleY = termHeight / frameHeight;

    return std::min(scaleX, scaleY);
}

int main(int argc, char** argv) {
    using namespace ftxui;
    using namespace std::chrono_literals;

    std::atexit(showCursor);
    std::signal(SIGINT, showCursor);
    cv::Mat videoFrame{};

    toggleCursor(OFF);
    ansiClearScreen();

    for (int frame{}; frame < static_cast<int>(vta::userVideo.getFrameCount()); frame++) {
        auto screen { Screen::Create(Terminal::Size()) };                       // Create a screen with size of current terminal
        vta::userVideo.getVideo().read(videoFrame);                                         // Reads 1st frame and place it to the testFrame
        const auto terminalSize { Terminal::Size() };
        const double scale { scalingFactor(videoFrame, terminalSize) };                  // Calculate scale for the frame

        // Timer representing rate between frames, or just FPS
        std::this_thread::sleep_for(std::chrono::duration<double>(vta::userVideo.getFpsDelay() / 2));
        cv::cvtColor(videoFrame, videoFrame, cv::COLOR_RGB2GRAY);               // Convert colors to GRAY
        cv::resize(videoFrame, videoFrame, cv::Size(), scale, scale * vta::ASCII_VERTICAL_SCALE);

        std::vector<Element> terminalFrame{};

        for (const auto& frameLine : frameConvertToAscii(videoFrame)) {
            terminalFrame.push_back(center(text(frameLine)));
        }

        Element frameT { center(vbox(terminalFrame))};
        Render(screen, frameT);
        ansiClearScreen();
        screen.Print();
    }

    ansiClearScreen();
    std::cout << "FPS: " << vta::userVideo.getFps() << '\n';
    std::cout << "Frame Count: " << vta::userVideo.getFrameCount() << std::endl;

    return 0;
}
