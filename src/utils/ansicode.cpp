#include <iostream>

namespace ansi {
    void clearScreen() {
        std::cout << "\033[2J\033[H"; // Cleans terminal
    }

    void moveTopLeft() {
        std::cout << "\033[H"; // Moves terminal cursor to the top-left corner
    }

    void toggleCursor(const bool cursorState) {
        if (cursorState) {
            std::cout << "\033[?25h"; // Show cursor
        } else {
            std::cout << "\033[?25l"; // Hide cursor
        }
    }

    void showCursor(int signal) {
        clearScreen();
        std::cout << "\033[?25h" << std::flush; // Show cursor

        std::exit(1);
    }

    void showCursor() {
        clearScreen();
        std::cout << "\033[?25h" << std::flush; // Show cursor
    }
}