We need to colour the triangle by drawing horizontal lines from one side of the triangle to the other. But how do we decide which side we need to start from.

![](<pics/sides.png>)

Its pretty clear that when we do this horizontal line sweeping there is one line that is drawn towards or from, in the image above we want horizontal lines to either start at the white lines of in the case of the smallest triangle end at the white line. 

By looking at it is clear that the white line is the one which has the maximum y "component". And to find that, basic bubble sort is sufficient.
```
void triangle(Vec3 p0, Vec3 p1, Vec3 p2, TGAColor colour, TGAImage *image){
        // sorting the points based on y coordinate 
        if (p0.y > p1.y) std::swap(p0, p1);
        if (p1.y > p2.y) std::swap(p1, p2);
        if (p0.y > p1.y) std::swap(p0, p1);

        line(p0, p1, colour, image); 
        line(p1, p2, colour, image); 
        line(p2, p0, white, image);
}
```
## Colouring the triangle

Consider the lower half of the triangle, we will be drawing horizontal lines that start from one of the lines and end at the other, and its much easier to do this when we think in terms of vectors, the diagram below is very self explanatory, we are merely scaling the vectors along those lines by how far up we go.

![](<pics/IMG_20250113_200635.jpg>)

```
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
}
```
[](<pics/bottom_half.png>)

I know I said bottom half, and this is the top half, I just didn't bother flipping the y-axis, display nonsense

And we can do something similar for the top half
```
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
```
![[full_triangle.png|500]]
