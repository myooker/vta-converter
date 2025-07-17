#ifndef VIDEO_H
#define VIDEO_H

#include <opencv4/opencv2/videoio.hpp>


class Video {
private:
    cv::VideoCapture m_video{"/home/myooker/iphone.mp4"};
    double m_frameCount{ 300 };
    double m_fps{ 30.0 };
public:
    explicit Video(const std::string& path)
        : m_video{ path }, m_frameCount{ m_video.get(cv::CAP_PROP_FRAME_COUNT) }, m_fps{ m_video.get(cv::CAP_PROP_FPS)}
    {
    }

    cv::VideoCapture& getVideo() { return m_video; }
    double getFrameCount() const { return m_frameCount; }
    double getFps() const { return m_fps; }
};



#endif //VIDEO_H
