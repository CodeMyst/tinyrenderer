#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

typedef struct VectorLong3 {
    long x;
    long y;
    long z;
} VectorLong3;

typedef struct Model {
    Vector3 vertices[5000];
    long verticesLength;
    VectorLong3 faces[5000];
    long facesLength;
} Model;

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

void loadModel(const char* path, Model* model) {
    FILE* file = fopen(path, "r");

    if (file == NULL) {
        printf("Failed to open model file.\n");
        exit(1);
    }

    model->verticesLength = 0;
    model->facesLength = 0;

    while (!feof(file)) {
        char c[2];
        fscanf(file, "%s ", c);
        // Lines in the format of `v x y z` specify a vertex
        if (c[0] == 'v' && c[1] == '\0') {
            float x = 0;
            float y = 0;
            float z = 0;

            fscanf(file, " %f %f %f\n", &x, &y, &z);

            model->vertices[model->verticesLength] = (Vector3){x, y, z};
            model->verticesLength++;
        // Lines in the format of `f x/x/x x/x/x x/x/x` specify a face
        // The first number in the groups specify the vertex index (starting at 1)
        } else if (c[0] == 'f' && c[1] == '\0') {
            long vert1 = 0;
            long vert2 = 0;
            long vert3 = 0;

            fscanf(file, " %ld/%*ld/%*ld %ld/%*ld/%*ld %ld/%*ld/%*ld\n", &vert1, &vert2, &vert3);

            model->faces[model->facesLength] = (VectorLong3){vert1, vert2, vert3};
            model->facesLength++;
        } else {
            fscanf(file, "%*[^\n]\n"); // skip line
        }
    }

    fclose(file);
}

void drawModel(Model model, float xOffset, float yOffset, float scale) {
    for (int i = 0; i < model.facesLength; i++) {
        VectorLong3 face = model.faces[i];

        // -1 because the obj format stores indices starting at 1
        Vector3 a = model.vertices[face.x - 1];
        Vector3 b = model.vertices[face.y - 1];
        Vector3 c = model.vertices[face.z - 1];

        // flip the model and apply the scale
        a.x *= -1 * scale;
        a.y *= -1 * scale;
        b.x *= -1 * scale;
        b.y *= -1 * scale;
        c.x *= -1 * scale;
        c.y *= -1 * scale;

        // translate the model by an offset
        a.x += xOffset;
        b.x += xOffset;
        c.x += xOffset;

        a.y += yOffset;
        b.y += yOffset;
        c.y += yOffset;

        drawLine(a.x, a.y, b.x, b.y, (Color){255, 255, 255, 255});
        drawLine(b.x, b.y, c.x, c.y, (Color){255, 255, 255, 255});
        drawLine(c.x, c.y, a.x, a.y, (Color){255, 255, 255, 255});
    }
}

int main() {
    InitWindow(800, 800, "tinyrenderer");

    SetTargetFPS(60);

    Model model = {0};
    loadModel("./assets/african_head.obj", &model);

    float xOffset = 800 / 2.0;
    float yOffset = 800 / 2.0;
    float scale = 800 / 2.0;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_A)) {
            xOffset -= 1;
        } else if (IsKeyDown(KEY_D)) {
            xOffset += 1;
        }

        if (IsKeyDown(KEY_W)) {
            yOffset -= 1;
        } else if (IsKeyDown(KEY_S)) {
            yOffset += 1;
        }

        if (IsKeyDown(KEY_Q)) {
            scale -= 1;
        } else if (IsKeyDown(KEY_E)) {
            scale += 1;
        }

        BeginDrawing();

        ClearBackground((Color){0, 0, 0, 255});

        DrawFPS(10, 10);

        drawModel(model, xOffset, yOffset, scale);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

