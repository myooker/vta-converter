#include "video.h"

#include <thread>
#include <opencv2/imgproc.hpp>

#include "engine/constants.h"
#include "ftxui/screen/screen.hpp"

#include "engine/constants.h"
#include "engine/scaling.h"
#include "ftxui/dom/elements.hpp"

std::vector<std::string> Video::frameToAscii(cv::Mat& videoFrame) {
    const std::size_t frameRowSize{ static_cast<std::size_t>( videoFrame.rows )};    // Get frame's row f(x) size
    const std::size_t frameColSize{ static_cast<std::size_t>( videoFrame.cols )};    // Get frame's column f(y) size
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
            pixelValue = videoFrame.at<uchar>(static_cast<int>(rowCount), static_cast<int>(colCount));
            asciiFrame[rowCount].push_back( vta::palette[getPaletteIndex(pixelValue)] );
        }
    }

    return asciiFrame;
}

void Video::playAscii(cv::Mat& videoFrame) {
    using namespace ftxui;
    using namespace std::chrono;
    using AsciiFrame = std::vector<std::string>;

    for (int frame{}; frame < static_cast<int>(getFrameCount()); frame++) {
        auto frameStart {steady_clock::now()};
        auto screen {Screen::Create(Terminal::Size()) };   // Create a screen with size of current terminal
        m_video.read(videoFrame);                   // Reads 1st frame and place it to the testFrame
        const auto terminalSize { Terminal::Size() };
        const double scale { scalingFactor(videoFrame, terminalSize) }; // Calculate scale for the frame

        cv::cvtColor(videoFrame, videoFrame, cv::COLOR_RGB2GRAY);               // Convert colors to GRAY
        cv::resize(videoFrame, videoFrame, cv::Size(), scale, scale * vta::ASCII_VERTICAL_SCALE);

        Elements terminalFrame{};   // Here we will store our ASCII rows

        // Read ASCII frame push it to Elements as Element(center(text(frameLine))):
        // Centered string (text) from frameLine
        for (AsciiFrame tempFrame = frameToAscii(videoFrame); const auto& frameLine : tempFrame) {
            terminalFrame.push_back(center(text(frameLine)));
        }
        const auto frameEnd {steady_clock::now()};
        const auto delay {frameEnd - frameStart};

        // Timer representing rate between frames, or just FPS
        std::this_thread::sleep_for(duration<double>(getFpsDelay() - delay));
        Element frameT { center(vbox(terminalFrame))};  // Use our Elements as one unite Element frameT
        Render(screen, frameT); // Render frameT on our screen
        // ansi::clearScreen();    // Clear terminal screen
        screen.Print(); // Print our screen to the terminal
    }
}
