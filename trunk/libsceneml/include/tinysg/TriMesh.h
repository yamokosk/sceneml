/*************************************************************************
 * TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version. The text of the GNU Lesser General
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for
 * more details.
 *
 *************************************************************************/

#ifndef _TRIMESH_H_FILE_
#define _TRIMESH_H_FILE_

// External includes
#include <vector>
#include <memory>

namespace TinySG
{

class TriMesh
{
public:
	struct Vertex
	{
		float x, y, z;
	};
	struct Face
	{
		unsigned int v1, v2, v3;
	};

	typedef std::vector<Vertex> VertexList;
	typedef std::vector<Face> FaceList;

	TriMesh() {};
	virtual ~TriMesh() {};

	// Call to reserve space in the vertex and face containers. Not
	// necessary but could improve performance.
	void init(unsigned int nv, unsigned int nf);

	inline unsigned int numVertices() {return (unsigned int)vertices_.size();}
	inline unsigned int numFaces() {return (unsigned int)faces_.size();}

	// For use with memcpy or other direct access methods
	//const Vertex* vertexData() const {return const_cast<Vertex*>(&vertices_[0]);}
	//const Face* faceData() const {return const_cast<Face*>(&faces_[0]);}
	const Vertex* vertexData() const {return &vertices_[0];}
	const Face* faceData() const {return &faces_[0];}
	uint8_t vertexStride() {return sizeof(TriMesh::Vertex);}
	uint8_t faceStride() {return sizeof(TriMesh::Face);}

	// Methods to add vertices and faces
	unsigned int addVertex(float x, float y, float z);
	unsigned int addVertex(const TriMesh::Vertex& v);
	unsigned int addFace(unsigned int v1, unsigned int v2, unsigned int v3);
	unsigned int addFace(const TriMesh::Face& f);

protected:
	VertexList vertices_;
	FaceList faces_;
};

typedef std::auto_ptr<TriMesh> TriMeshPtr;

} // End TinySG namespace

#endif
