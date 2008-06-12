#ifndef BOUNDINGMESH_H
#define BOUNDINGMESH_H

#include "Primitive.h"
#include "OBJImport.h"

// convex bounding geometery
// supports only quad/tri faces 
// for now 
 
class BoundingMesh{
public:
	POLYHEDRON* mesh;
	BoundingMesh(char* file_name);
	~BoundingMesh();
	POLYHEDRON* getPoly();
};

#endif
