#include "Primitive.h"

POLYHEDRON::POLYHEDRON(){
	this->vertex_count = 0;
	this->index_count = 0;
	index_stride = 0;
	vertex_stride = 0;
	filename = "_NODATA_";
}

POLYHEDRON::~POLYHEDRON(){
	delete [] vertices;
	delete [] indices;
}
