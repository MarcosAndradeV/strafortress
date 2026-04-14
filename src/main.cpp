#include <raylib.h>
#include "world.hpp"

#include "../include/logging.h"
#include "../include/config.h"
#include "../include/colors.h"


int main()
{
    init_logging();
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
    SetTargetFPS(60);

    World world(1024, 1024);
    world.Generate(42);

    // Default reveal 32x32 center sector
    int centerX = 1024 / 2;
    int centerY = 1024 / 2;
    world.RevealSector(centerX - 16, centerY - 16, 32, 32);

    Camera2D camera = { 0 };
    camera.target = { (float)centerX * TILE_SIZE, (float)centerY * TILE_SIZE };
    camera.offset = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while(!WindowShouldClose()) {
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

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        BeginMode2D(camera);
        world.DrawWorld(camera, WINDOW_WIDTH, WINDOW_HEIGHT);
        EndMode2D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

#define NOB_IMPLEMENTATION
#include "../include/nob.h"
