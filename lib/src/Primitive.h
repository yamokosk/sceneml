#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>

typedef float* VERTEX3D;
typedef unsigned int* INDEX;
 
struct POLYHEDRON {
	long vertex_count, index_count;
	short vertex_stride;
	VERTEX3D vertices;
	short index_stride;
	INDEX indices;
	std::string filename;
	POLYHEDRON();
	~POLYHEDRON();
};

#endif
