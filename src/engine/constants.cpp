#include <cstddef>
#include "constants.h"

std::size_t getPaletteIndex(const unsigned int pixelColor) {
    return (pixelColor * (vta::palette.size()-1) / MAX_GRAY_VALUE);
}
