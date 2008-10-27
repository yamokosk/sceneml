#include "BoundingMesh.h"

BoundingMesh::BoundingMesh(char* filename){
	this->mesh = new POLYHEDRON();
	importOBJ(filename, this->mesh);
}
BoundingMesh::~BoundingMesh(){
	delete mesh;
}

POLYHEDRON* BoundingMesh::getPoly(){
	return mesh;
}