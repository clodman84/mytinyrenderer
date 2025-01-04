#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

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

#endif //__MODEL_H__
