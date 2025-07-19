#ifndef VIDEO_H
#define VIDEO_H

#include <opencv4/opencv2/videoio.hpp>


class Video {
private:
    cv::VideoCapture m_video{"/home/myooker/bluezenith.mp4"};
    double m_frameCount{ 333 };
    double m_fps{ 33.3 };
    const std::chrono::duration<double> m_fpsDelay{ 1.0 / m_video.get(cv::CAP_PROP_FPS)};
public:
    Video()
        : m_frameCount{ m_video.get(cv::CAP_PROP_FRAME_COUNT)}, m_fps{ m_video.get(cv::CAP_PROP_FPS)}
    {
    }

    explicit Video(const std::string& path)
        : m_video{ path }, m_frameCount{ m_video.get(cv::CAP_PROP_FRAME_COUNT) }, m_fps{ m_video.get(cv::CAP_PROP_FPS)}
    {
    }

    cv::VideoCapture& getVideo() { return m_video; }
    double getFrameCount() const { return m_frameCount; }
    double getFps() const { return m_fps; }
    auto getFpsDelay() const { return m_fpsDelay; }
};



#endif //VIDEO_H
