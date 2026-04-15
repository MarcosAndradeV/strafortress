#include <raylib.h>
#include "context.hpp"
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
    Context ctx(centerX, centerY);

    while(!WindowShouldClose()) {
        ctx.UpdateCamera();

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        auto camera = ctx.GetCamera();
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
