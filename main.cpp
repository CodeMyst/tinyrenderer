#include "geometry.h"
#include "model.h"
#include "tgaimage.h"
#include <cmath>
#include <fstream>
#include <type_traits>
#include <vector>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor color) {
    bool steep = false;

    // line is more high than it's wide
    if (std::abs(t0.x - t1.x) < std::abs(t0.y - t1.y)) {
        steep = true;
        std::swap(t0.x, t0.y);
        std::swap(t1.x, t1.y);
    }

    // make sure starting point is always lower
    if (t0.x > t1.x) {
        std::swap(t0.x, t1.x);
        std::swap(t0.y, t1.y);
    }

    int dx = t1.x - t0.x;
    int dy = t1.y - t0.y;
    int derr = std::abs(dy) * 2;
    int err = 0;
    int y = t0.y;

    int yincr = t1.y > t0.y ? 1 : -1;

    for (int x = t0.x; x <= t1.x; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }

        err += derr;

        if (err > dx) {
            y += yincr;
            err -= dx * 2;
        }
    }
}

void bottomFlatTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
    float invslope1 = (float)(t1.x - t0.x) / (float)(t1.y - t0.y);
    float invslope2 = (float)(t2.x - t0.x) / (float)(t2.y - t0.y);

    float curx1 = t0.x;
    float curx2 = t0.x;

    for (int y = t0.y; y <= t1.y; y++) {
        line(Vec2i(curx1, y), Vec2i(curx2, y), image, color);

        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void topFlatTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
    float invslope1 = (float)(t2.x - t0.x) / (float)(t2.y - t0.y);
    float invslope2 = (float)(t2.x - t1.x) / (float)(t2.y - t1.y);

    float curx1 = t2.x;
    float curx2 = t2.x;

    for (int y = t2.y; y > t0.y; y--) {
        line(Vec2i(curx1, y), Vec2i(curx2, y), image, color);

        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);

    if (t1.y == t2.y) {
        bottomFlatTriangle(t0, t1, t2, image, color);
    } else if (t0.y == t1.y) {
        topFlatTriangle(t0, t1, t2, image, color);
    } else {
        Vec2i t3 = Vec2i((int)(t0.x + ((float)(t1.y - t0.y) / (float)(t2.y - t0.y)) * (t2.x - t0.x)), t1.y);
        bottomFlatTriangle(t0, t1, t3, image, color);
        topFlatTriangle(t1, t3, t2, image, color);
    }
}

void wireframe(Model& model, int width, int height, TGAImage& image) {
    for (int i = 0; i < model.nfaces(); i++) {
        std::vector<int> face = model.face(i);

        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model.vert(face[j]);
            Vec3f v1 = model.vert(face[(j + 1) % 3]);

            int x0 = (v0.x + 1) * width / 2;
            int y0 = (v0.y + 1) * height / 2;
            int x1 = (v1.x + 1) * width / 2;
            int y1 = (v1.y + 1) * height / 2;

            line(Vec2i(x0, y0), Vec2i(x1, y1), image, white);
        }
    }
}

int main(int argc, char** argv) {
    Model model("obj/african_head.obj");

    const int width = 1000;
    const int height = 1000;

	TGAImage image(width, height, TGAImage::RGB);
    
    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)}; 
    triangle(t0[0], t0[1], t0[2], image, red); 
    triangle(t1[0], t1[1], t1[2], image, white); 
    triangle(t2[0], t2[1], t2[2], image, green);

	image.flip_vertically(); // origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}

