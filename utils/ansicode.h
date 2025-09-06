#ifndef ANSICODE_H
#define ANSICODE_H

namespace ansi {
    enum CursorToggle {
        OFF = 0,
        ON = 1,
    };
    void clearScreen();
    void moveTopLeft();
    void toggleCursor(const bool cursorState);
    void showCursor(int signal);
    void showCursor();
}

#endif //ANSICODE_H
