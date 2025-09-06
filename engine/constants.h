#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string_view>

namespace vta {
    constexpr std::string_view palette{" .:-=+*#%@"};       // From lightest to darker
    constexpr std::string_view dpalette{"@%#*+=-:. "};      // From darker to lightes

    // As character taller than a pixel, we use this constant to decrease video's height
    constexpr double ASCII_VERTICAL_SCALE = 0.5;
}

enum Constants {
    MAX_GRAY_VALUE = 255,
};

std::size_t getPaletteIndex(const unsigned int pixelColor);

#endif //CONSTANTS_H
