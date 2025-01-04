As outlined by the Wavefront OBJ File Format we will have to read the array of vertices that have v as the first character in the line

```
v 0.437500 0.164063 0.765625
```

After which we can look at the array of faces that look like this

```
f 61/1/1 65/2/2 49/3/3
```

where the first number in the blocks separated by spaces refer to the index of the vertex that makes up that face, in this case, vertex number 61, 65 and 49 make up this face, (.obj files begin indexing from 1 so we should look at index 60, 64 and 48 in our array).

## Writing a basic parser

The first thing we'll do is define a struct `vec3` with some usual mathematical operations to represent the vertices as they are actual vectors in 3D space and we might want to do some regular vector maths with them.

```
struct Vec3 {
	// A vector is just a struct of three floats
	struct {float x, y, z;};
	// with default value 0
	Vec3() : x(0), y(0), z(0) {}
	Vec3(float _x, float _y, float _z) : x(_x),y(_y),z(_z) {}
	
	// defining a cross product
	inline Vec3 operator ^(const Vec3 &v) const { return Vec3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	// dot product
	inline float operator *(const Vec3 &v) const { return x*v.x + y*v.y + z*v.z; }
	// vector addition
	inline Vec3 operator +(const Vec3 &v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
	// vector subtraction
	inline Vec3 operator -(const Vec3 &v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
	// scalar multiplication
	inline Vec3 operator *(float f) const { return Vec3(x*f, y*f, z*f); }
	// length of the vector
	float norm () const { return std::sqrt(x*x+y*y+z*z); }
	// normalising a vector
	Vec3 &normalize(float l=1) { *this = (*this)*(l/norm()); return *this; }
	
	// A way to display the vector
	friend std::ostream& operator<<(std::ostream& s, Vec3 v){
		s << "(" << v.x << ", "<< v.y << ", "<< v.z << ")\n";
		return s;
	} 
};
```

All right, now that we have vector like we are used to in mathematics we can move on to representing models.
```
class Model {
private:
	// all the vertices of the model
	std::vector<Vec3> verts_;
	// faces are not limited to three vertices btw, which is we use vectors
	std::vector<std::vector<int>> faces_;
public:
	Model(const char *filename);
	~Model();
	// number of vertices and faces
	int nverts();
	int nfaces();
	// ways to access a vertex by index
	Vec3 vert(int i);
	// ways to access a face by index
	std::vector<int> face(int idx);
};
```

Now we need a program a way to construct this model and define all the methods we've declared here.
```
Model::Model(const char *filename): verts_(), faces_(){
        // constructing the model object
        std::ifstream in;
        in.open(filename);
        if (in.fail()) return;
        std::string line;

        while (!in.eof()){
                std::getline(in, line);
                std::istringstream iss(line.c_str());
                char trash;
                if (!line.compare(0, 2, "v ")){
                        // throwing away the 'v ' part
                        iss >> trash;
                        Vec3 v;
                        for (int i=0; i<3; i++) iss>>v.raw[i];
                        verts_.push_back(v);
                } else if (!line.compare(0, 2, "f ")) {
                        std::vector<int> f;
                        int itrash, idx;
                        iss >> trash;
                        // taking the first value and throwing away the remaining 2 indices
                        // we are assuming that there will always be groups of three numbers 
                        // separated by / but the obj format allows those to be omitted 
                        // so this is not particularly good
                        while (iss >> idx >> trash >> itrash >> trash >> itrash){
                                idx--; // .obj indexing starts at 1
                                f.push_back(idx);
                        }
                        faces_.push_back(f);
                }
        }
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3 Model::vert(int i) {
    return verts_[i];
}
```

Now we have a way to read wireframes.

## Rendering

![alt](<Suzanne.jpeg>)

```

int main(int argc, char** argv) {
        int width = 500, height = 500;
        TGAImage image(width, height, TGAImage::RGB);
        Model monke("./suzanne.obj");
        for (int i=0; i < monke.nfaces(); i++) {
                std::vector<int> face = monke.face(i);
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
```

This is extremely straightforward so I am not gonna bother explaining whats going on in detail, it just connects the line and draws them using the Bresenham’s Line Drawing Algorithm

Suzanne is upside down, but that is because while .obj uses coordinates that are the right way up, the coordinate system of displays is upside down.

Moreover, since we are completely ignoring the z value of the vector we are effectively looking at a shadow of Suzanne (orthographic projection) that is cast from a light source at infinity, which isn't how our eyes perceive things and it is also why any amateur artist will scoff at you if you draw cubes with all sides of equal length (if theyre an asshole that is).

Moreover the image looks funky as hell, there are clearly too many lines and we are rendering too much of the scene, we can make this better by discarding the faces that are pointing away from the camera.

ssloy/tinyrenderer uses z-buffers in the next few lessons, but I am not there yet and I want to use simple normal vector based culling right now and look at Suzanne in all her glory.

The idea is to find a normal vector of the triangle, and then the dot product with the camera vector should be positive is the normal is lying along it

```
int main(int argc, char** argv) {
        int width = 500, height = 500;
        TGAImage image(width, height, TGAImage::RGB);
        Model monke("./suzanne.obj");
        Vec3 camera{0, 0, 1};
        for (int i=0; i < monke.nfaces(); i++) {
                std::vector<int> face = monke.face(i);
                // deciding if it should be drawn or not
                Vec3 normal = (monke.vert(face[1]![[Suzanne1.jpeg]]) - monke.vert(face[0])) ^ (monke.vert(face[2]) - monke.vert(face[0]));
                if (camera*normal > 0) continue;
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
```

![suzaaaaannneeee](<Suzanne1.jpeg>)
