#include "raylib.h"

int main() {
    InitWindow(1280, 720, "tinyrenderer");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground((Color) {0, 0, 0, 255});

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
