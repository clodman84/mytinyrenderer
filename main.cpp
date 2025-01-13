#include "geometry.h"
#include "tgaimage.h"
// #include "model.h"
#include <algorithm>
#include <utility>
#include <cmath>
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line(int x0, int y0, int x1, int y1, TGAColor colour, TGAImage *image){
        // Bresenham Algorithm from Wikipedia (not exactly but pretty much the same)
        bool steep = false;
        if (std::abs(y1-y0)> std::abs(x1-x0)){
                std::swap(x0, y0);
                std::swap(x1, y1);
                steep = true;
        }
        if (x0 > x1) {
                std::swap(x0, x1);
                std::swap(y0, y1);
        }
        int yi = 1;
        int dx = x1 - x0;
        int dy = y1 - y0;
        if (dy < 0){
                yi = -1;
                dy = -dy;
        }
        int D =  2*dy - dx;
        int y = y0;

        for (int x = x0; x <= x1; x++){
                if(steep) image->set(y, x, colour); 
                else image->set(x, y, colour);
                if (D > 0){
                        y += yi;
                        D -= 2*dx;
                }
                D += 2*dy;
        }
}

void line(Vec3 p0, Vec3 p1, TGAColor colour, TGAImage *image){
        line(p0.x, p0.y, p1.x, p1.y, colour, image);
}

void triangle(Vec3 p0, Vec3 p1, Vec3 p2, TGAColor colour, TGAImage *image){
        // sorting the points based on y coordinate 
        if (p0.y > p1.y) std::swap(p0, p1);
        if (p1.y > p2.y) std::swap(p1, p2);
        if (p0.y > p1.y) std::swap(p0, p1);

        // line(p0, p1, colour, image); 
        // line(p1, p2, colour, image); 
        // line(p2, p0, white, image);

        int height = p2.y - p0.y;  // used to scale the vector along p0->p2
        int bottom_half_height = p1.y - p0.y;  // used to scale the vector along p0->p1

        for (int y = p0.y; y <= p1.y; y++){
                // the x value for the line joining p0 and p1
                float scaler_02 = (y - p0.y) / height;
                float scaler_01 = (y - p0.y) / bottom_half_height;
                Vec3 vec_02 = p0 + (p2 -  p0) * scaler_02;
                Vec3 vec_01 = p0 + (p1 -  p0) * scaler_01;
                if (vec_01.x > vec_02.x) std::swap(vec_01, vec_02);
                for (int x = vec_01.x; x <= vec_02.x; x++){
                        image->set(x, y, white);
                }
        }

        // top half
        int top_half_height = p2.y - p1.y;  // used to scale the vector along p1->p2
        for (int y = p1.y; y <= p2.y; y++){
                float scaler_02 = (y - p0.y) / height;
                float scaler_12= (y - p1.y) / top_half_height;
                Vec3 vec_02 = p0 + (p2 -  p0) * scaler_02;
                Vec3 vec_12 = p1 + (p2 -  p1) * scaler_12;
                if (vec_12.x > vec_02.x) std::swap(vec_12, vec_02);
                for (int x = vec_12.x; x <= vec_02.x; x++){
                        image->set(x, y, white);
                }
        }


}

int main(int argc, char** argv) {
        int width = 200, height = 200;
        TGAImage image(width, height, TGAImage::RGB);
        // Model monke("./suzanne.obj");
        // Vec3 camera{0, 0, -1};
        // for (int i=0; i < monke.nfaces(); i++) {
        //         std::vector<int> face = monke.face(i);
        //         // deciding if it should be drawn or not
        //         Vec3 normal = (monke.vert(face[1]) - monke.vert(face[0])) ^ (monke.vert(face[2]) - monke.vert(face[0]));
        //         if (camera*normal >= 0) continue;
        //         for (int j=0; j < 3; j++){
        //                 Vec3 v0 = monke.vert(face[j]);
        //                 Vec3 v1 = monke.vert(face[(j+1)%3]);
        //                 int x0 = (v0.x+1.)*width/2.; 
        //                 int y0 = (v0.y+1.)*height/2.; 
        //                 int x1 = (v1.x+1.)*width/2.; 
        //                 int y1 = (v1.y+1.)*height/2.; 
        //                 line(x0, y0, x1, y1, white, &image);
        //         }
        // }
        Vec3 triangle1[3] = {Vec3{10, 70, 0}, Vec3{50, 160, 0}, Vec3{70, 80, 0}};
        Vec3 triangle2[3] = {Vec3{180, 50, 0}, Vec3{150, 1, 0}, Vec3{70, 180, 0}};
        Vec3 triangle3[3] = {Vec3{180, 150, 0}, Vec3{120, 160, 0}, Vec3{130, 180, 0}};

        triangle(triangle1[0], triangle1[1], triangle1[2], red, &image);
        triangle(triangle2[0], triangle2[1], triangle2[2], red, &image);
        triangle(triangle3[0], triangle3[1], triangle3[2], red, &image);

        image.write_tga_file("output.tga");
        return 0;
}
