// Using raylib only to open the window and draw pixels
// That's why I didn't include the entire raylib.h file

#include <stdbool.h>

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

void InitWindow(int width, int height, const char* title);
void CloseWindow(void);
bool WindowShouldClose(void);
void ClearBackground(Color color);
void BeginDrawing(void);
void EndDrawing(void);
void DrawPixel(int posX, int posY, Color color);
void SetTargetFPS(int fps);
void DrawFPS(int posX, int posY);

