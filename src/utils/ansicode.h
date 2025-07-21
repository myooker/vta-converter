#ifndef ANSICODE_H
#define ANSICODE_H

namespace ansi {
    void clearScreen();
    void moveTopLeft();
    void toggleCursor(const bool cursorState);
    void showCursor(int signal);
    void showCursor();
}

#endif //ANSICODE_H
