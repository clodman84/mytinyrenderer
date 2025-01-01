#include "tgaimage.h"
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
        TGAImage image(100, 100, TGAImage::RGB);
        for (int y = 0; y <=50; y++){
                int x = std::sqrt(2500 - y*y);
                line(50 - x, 50 + y, 50 + x, 50 + y, red, &image);
        }
        image.write_tga_file("output.tga");
        return 0;
}
