#include <iostream>
#include <chrono>

#include "../video.h"

namespace DEBUG {

    namespace VIDEO {
        using namespace std::chrono_literals;

        void printFrameRate(const Video& video) {
            std::cout << "[DEBUG/VIDEO] m_fps: " << video.getFps() << '\n';
        }

        void printFrameCount(const Video& video) {
            std::cout << "[DEBUG/VIDEO] m_frameCount: " << video.getFrameCount() << '\n';
        }

        void printFpsDelay(const Video& video) {
            std::cout << "[DEBUG/VIDEO] m_fpsDelay: " << 1 / video.getFps() << '\n';
        }
    }

}
