#include <iostream>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/videoio/videoio.hpp>


int main() {
    cv::VideoCapture video{"/home/myooker/Downloads/output.mp4"};
    cv::Mat frame{};

    if (!video.isOpened()) {
        std::cerr << "Unable to open video file" << std::endl;
    }

    // cv::namedWindow("Image", cv::WINDOW_NORMAL);
    // cv::resizeWindow("Image", 1080, 1920);
    // cv::imshow("Image",frame);

    while (true) {
        video.read(frame);

        if (frame.empty()) {
            std::cerr << "Error! Blank frame grabbed\n";
            break;
        }
        cv::namedWindow("Video", cv::WINDOW_NORMAL);
        cv::resizeWindow("Video", 504, 896);
        cv::imshow("Video", frame);
        if (cv::waitKey(15) >= 0)
            break;
    }

    // // Exit program only when 'q' is pressed
    // while (true) {
    //     int key = cv::waitKey(0);
    //     if (key == 'q') break;
    // }

    video.release();

    return 0;
}