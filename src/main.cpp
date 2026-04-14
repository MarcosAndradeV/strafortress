#include "../include/logging.h"
#include "../include/config.h"
#include "../include/colors.h"

#include <raylib.h>

int main()
{
    init_logging();
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

#define NOB_IMPLEMENTATION
#include "../include/nob.h"
