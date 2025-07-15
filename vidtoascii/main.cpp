#include <iostream>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/videoio/videoio.hpp>


int main() {
    cv::VideoCapture video{"/home/myooker/Downloads/output.mp4"};
    cv::Mat frame{};

    if (!video.isOpened()) {
        std::cerr << "Unable to open video file" << std::endl;
    }

    video.read(frame);

    cv::namedWindow("Image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Image", 1080, 1920);
    cv::imshow("Image",frame);

    // Exit program only when 'q' is pressed
    while (true) {
        int key = cv::waitKey(0);
        if (key == 'q') break;
    }

    video.release();

    return 0;
}