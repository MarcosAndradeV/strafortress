#include "context.hpp"
#include "../include/config.h"
#include "world.hpp"
#include <raylib.h>

Context::Context(int centerX, int centerY) {
    camera.target = { (float)centerX * TILE_SIZE, (float)centerY * TILE_SIZE };
    camera.offset = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

Camera2D Context::GetCamera() {
    return camera;
}

void Context::UpdateCamera() {
    float dt = GetFrameTime();

    float speed = 500.0f / camera.zoom;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) camera.target.x += speed * dt;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) camera.target.x -= speed * dt;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) camera.target.y += speed * dt;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) camera.target.y -= speed * dt;

    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        camera.zoom += wheel * 0.1f;
        if (camera.zoom < 0.1f) camera.zoom = 0.1f;
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
    }
}
