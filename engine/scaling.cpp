#include <opencv2/core/mat.hpp>
#include <ftxui/screen/terminal.hpp>

#include "constants.h"
#include "scaling.h"

double scalingFactor(const cv::Mat& videoFrame, const ftxui::Dimensions& screen) {
    const auto termWidth  = static_cast<double>(screen.dimx);
    const auto termHeight = static_cast<double>(screen.dimy);

    const auto frameWidth  = static_cast<double>(videoFrame.cols);
    const auto frameHeight = static_cast<double>(videoFrame.rows) * vta::ASCII_VERTICAL_SCALE;

    const double scaleX = termWidth / frameWidth;
    const double scaleY = termHeight / frameHeight;

    return std::min(scaleX, scaleY);
}
