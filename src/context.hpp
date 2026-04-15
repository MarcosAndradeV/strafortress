#pragma once
#include <raylib.h>

class Context {
public:
    Context(int centerX, int centerY);

    void UpdateCamera();
    Camera2D GetCamera();
private:
    Camera2D camera;
};
