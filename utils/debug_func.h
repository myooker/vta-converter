#ifndef DEBUG_FUNC_H
#define DEBUG_FUNC_H

#include "../video.h"

namespace DEBUG {

    namespace VIDEO {
        void printFrameRate(const Video& video);

        void printFrameCount(const Video& video);

        void printFpsDelay(const Video& video);
    }

}

#endif //DEBUG_FUNC_H
