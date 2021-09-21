// Lesson 1: Bresenham's Line Drawing Algorithm

#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model *model = NULL;
const int width = 800;
const int height = 800;



// // First Attempt
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     for (float t=0; t <1.; t+=0.01) {
//         int x = x0 + (x1-x0)*t;
//         int y = y0 + (y1-y0)*t;
//         image.set(x, y, color);
//     }
// }

// // Second Attempt
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     for (int x=x0; x<=x1; x++) {
//         float t = (x-x0) / (float)(x1-x0);
//         int y = y0*(1.-t) + y1*t;
//         image.set(x, y, color);
//     }
// }

// // Third Attempt
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     bool steep = false;
//     if (std::abs(x0-x1)<std::abs(y0-y1)) { // in the line is steep we transpose the image
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         steep = true;
//     }
//     if (x0>x1) { // make it left-to-right
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }
//     for (int x=x0; x<=x1; x++) {
//         float t = (x-x0)/(float)(x1-x0);
//         int y = y0*(1.-t) + y1*t;
//         if (steep) {
//             image.set(y, x, color); // if transposed, de-transpose
//         } else {
//             image.set(x, y, color);
//         }
//     }
// }

// // Fourth attempt
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     bool steep = false;
//     if (std::abs(x0-x1)<std::abs(y0-y1)) {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         steep = true;
//     }
//     if (x0>x1) {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }
//     int dx = x1 - x0;
//     int dy = y1 - y0;
//     float derror = std::abs(dy/float(dx));
//     float error = 0;
//     int y = y0;
//     for (int x=x0; x<=x1; x++) {
//         if (steep) {
//             image.set(y, x, color);
//         } else {
//             image.set(x, y, color);
//         }
//         error += derror;
//         if (error>.5) {
//             y += (y1>y0?1:-1);
//             error -= 1.;
//         }
//     }
// }

// // Fifth attemp
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     bool steep = false;
//     if (std::abs(x0-x1)<std::abs(y0-y1)) {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         steep = true;
//     }
//     if (x0>x1) {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }
//     int dx = x1-x0;
//     int dy = y1-y0;
//     int derror2 = std::abs(dy)*2;
//     int error2 = 0;
//     int y = y0;
//     for (int x=x0; x<=x1; x++) {
//         if (steep) {
//             image.set(y, x, color);
//         } else {
//             image.set(x, y, color);
//         }
//         error2 += derror2;
//         if (error2 > dx) {
//             y += (y1>y0?1:-1);
//             error2 -= dx*2;
//         }
//     }
// }

// Fifth attemp + other optimizations
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;

    const int yincr = (y1>y0?1:-1);

    if (steep) {
        for (int x=x0; x<=x1; x++) {

            image.set(y, x, color);

            error2 += derror2;
            if (error2 > dx) {
                y += yincr;
                error2 -= dx*2;
            }
        }
    } else {
        for (int x=x0; x<=x1; x++) {

            image.set(x, y, color);

            error2 += derror2;
            if (error2 > dx) {
                y += yincr;
                error2 -= dx*2;
            }
        }
    }

}


int main(int argc, char** argv) {
    if (argc==2){
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }


    TGAImage image(width, height, TGAImage::RGB);
    // render model
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j=0; j<3; j++) {
            Vec3f v0 = model->vert(face[j]); 
            Vec3f v1 = model->vert(face[(j+1)%3]);
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            line(x0, y0, x1, y1, image, white);
        }
    }



    image.flip_vertically(); // image origin and bottom left
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}





