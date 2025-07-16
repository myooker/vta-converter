#include <iostream>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>


namespace vta {
    constexpr std::string_view palette{" .:-=+*#%@"}; // From lightest to darker
    const cv::Mat frame{ cv::imread("/home/myooker/Pictures/badapple.jpg", cv::IMREAD_REDUCED_GRAYSCALE_8) };
}

enum Constants {
    MAX_GRAY_VALUE = 255,
};

std::size_t getPaletteIndex(const unsigned int pixelColor) {
    return (pixelColor * (vta::palette.size()-1) / MAX_GRAY_VALUE);
}

std::string convertRow(const cv::Mat& frame, int govno) {
    if (frame.empty()) {
        std::cerr << "Error! No image detected!\nExiting the program..." << std::endl;
        std::exit(1);
    }

    uchar* ptr = frame.data;
    const std::size_t rowSize{ static_cast<std::size_t>(frame.rows - 1) };
    std::string asciiRow{};

    for (std::size_t i{}; i <= rowSize; i++) {
        // constexpr int defaultRow{ 0 };
        unsigned int pixelValue{};
        pixelValue = frame.at<uchar>(govno, static_cast<int>(i));
        asciiRow.push_back(vta::palette[getPaletteIndex(pixelValue)]);
    }

    return asciiRow;
}

int main() {
    std::cout << "Number of rows: " << vta::frame.rows << '\n';
    std::cout << "Number of columns: " << vta::frame.cols << '\n';

    for (int i{}; i < vta::frame.cols; i++) {
        std::cout << convertRow(vta::frame, i) << '\n';
    }

    // int key{};
    // while (true) {
    //     key = cv::waitKey(0);
    //     if (key == 'q') {
    //         // video.release();
    //         std::exit(0);
    //     }
    // }


    return 0;
}
