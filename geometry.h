#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <ostream>

struct Vec3 {
	// A vector is just a struct of three floats
	union{
		struct {float x, y, z;};
		// this bit makes it easy to put values in (like loops)
		float raw[3];
	};
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

#endif //__GEOMETRY_H__
