#include "geometry.h"
#include "tgaimage.h"
#include "model.h"
#include <utility>
#include <cmath>

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

int main(int argc, char** argv) {
        int width = 500, height = 500;
        TGAImage image(width, height, TGAImage::RGB);
        Model monke("./suzanne.obj");
        Vec3 camera{0, 0, -1};
        for (int i=0; i < monke.nfaces(); i++) {
                std::vector<int> face = monke.face(i);
                // deciding if it should be drawn or not
                Vec3 normal = (monke.vert(face[1]) - monke.vert(face[0])) ^ (monke.vert(face[2]) - monke.vert(face[0]));
                if (camera*normal < 0) continue;
                for (int j=0; j < 3; j++){
                        Vec3 v0 = monke.vert(face[j]);
                        Vec3 v1 = monke.vert(face[(j+1)%3]);
                        int x0 = (v0.x+1.)*width/2.; 
                        int y0 = (v0.y+1.)*height/2.; 
                        int x1 = (v1.x+1.)*width/2.; 
                        int y1 = (v1.y+1.)*height/2.; 
                        line(x0, y0, x1, y1, white, &image);
                }
        }
        image.write_tga_file("output.tga");
        return 0;
}
