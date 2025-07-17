#ifndef ANIMATION_H
#define ANIMATION_H

#include <string_view>
#include <vector>

class Animation {
private:
    double m_fps{ 30.0f };
    double m_fpsCount{ 300 };
    std::vector<std::vector<std::string>> m_animation{}; /*
        ^           ^           ^
  Full Animation  Frame     Frame's row
*/
public:


    void setAnimationFps(double fps, double fpsCount);
};



#endif //ANIMATION_H
