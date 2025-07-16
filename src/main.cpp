#include <iostream>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>

int main() {
    cv::VideoCapture video{"/home/myooker/Downloads/output.mp4"};
    cv::Mat frame{};

    if (!video.isOpened()) {
        std::cerr << "Unable to open video file" << std::endl;
    }

    const double videoFps{ video.get(cv::CAP_PROP_FPS)};
    std::cout << "video's fps: " << videoFps << '\n';

    while (true) {
        video.read(frame); // Reads video's frame and place it to frame (as an out parameter)
        cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY); // Convert RGB color of video's frame to GRAY

        if (frame.empty()) {
            std::cerr << "Error! Blank frame grabbed\n";
            break;
        }
        cv::namedWindow("Video", cv::WINDOW_NORMAL);
        cv::resizeWindow("Video", 504, 896);
        cv::imshow("Video", frame);
        if (cv::waitKey(15) >= 0) // Have no idea
            break;
    }

    video.release();

    return 0;
}
