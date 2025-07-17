#include <iostream>
#include <thread>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>

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
    // As character taller than a pixel, we use this constant to decrease video's height
    constexpr double ASCII_VERTICAL_PROP = 0.5;

    constexpr double REDUCE_SCALE_0 = 1;
    constexpr double REDUCE_SCALE_2 = 0.5;
    constexpr double REDUCE_SCALE_4 = 0.25;
    constexpr double REDUCE_SCALE_8 = 0.125;
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

// This function needed for std::atexit() to make the program restore cursor state to ON
// After the program exit, killing, terminates and etc...
void showCursor() {
    std::cout << "\033[?25h" << std::flush; // Show cursor
}

int main() {
    std::atexit(showCursor);

    constexpr double VIDEO_SCALE { vta::REDUCE_SCALE_4 };   // ...SCALE_0 - No scaling; SCALE_2 - reduce scale by 1/2; SCALE_4 - reduce scale by 1/4...
    Video userVideo{ /*getPath()*/ };
    // Animation asciiAnimation{};
    cv::Mat testFrame{};

    toggleCursor(OFF);
    ansiClearScreen();

    for (double frame{}; frame < userVideo.getFrameCount(); frame++) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(16.67ms);          // Timer representing rate between frames, or just FPS
        userVideo.getVideo().read(testFrame);     // Reads 1st frame and place it to the testFrame
        cv::cvtColor(testFrame, testFrame, cv::COLOR_RGB2GRAY);
        cv::resize(testFrame, testFrame, cv::Size(), VIDEO_SCALE, VIDEO_SCALE * vta::ASCII_VERTICAL_PROP);

        frameConvertToAscii(testFrame);
        ansiMoveTopLeft();
    }

    ansiClearScreen();
    toggleCursor(ON);
    std::cout << "FPS: " << userVideo.getFps() << '\n';
    std::cout << "Frame Count: " << userVideo.getFrameCount() << std::endl;

    return 0;
}
