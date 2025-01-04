#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"


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
