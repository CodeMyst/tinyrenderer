#include "raylib.h"
#include <math.h>
#include <stdlib.h>

void drawLine(int x0, int y0, int x1, int y1, Color color) {
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1)) {
        steep = true;

        int tmp = x0;
        x0 = y0;
        y0 = tmp;

        tmp = x1;
        x1 = y1;
        y1 = tmp;
    }

    if (x0 > x1) {
        int tmp = x0;
        x0 = x1;
        x1 = tmp;

        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror = abs(dy) * 2;
    int error = 0;

    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            DrawPixel(y, x, color);
        } else {
            DrawPixel(x, y, color);
        }

        error += derror;
        if (error >= dx) {
            y += (y1 > y0 ? 1 : -1);
            error -= dx * 2;
        }
    }
}

int main() {
    InitWindow(1280, 720, "tinyrenderer");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground((Color) {0, 0, 0, 255});

        DrawFPS(10, 10);

        drawLine(13, 20, 80, 40, (Color){255, 255, 255, 255}); 
        drawLine(20, 13, 40, 80, (Color){255, 0, 0, 255}); 
        drawLine(80, 40, 13, 20, (Color){255, 255, 255, 255});

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

