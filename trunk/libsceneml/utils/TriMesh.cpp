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

#include "TriMesh.h"


namespace TinySG
{

void TriMesh::init(unsigned int nv, unsigned int nf)
{
	vertices_.reserve(nv);
	faces_.reserve(nf);
}

unsigned int TriMesh::addVertex(float x, float y, float z)
{
	Vertex v; v.x = x; v.y = y; v.z = z;
	vertices_.push_back( v );
	return numVertices()-1;
}

unsigned int TriMesh::addVertex(const TriMesh::Vertex& v)
{
	vertices_.push_back( v );
	return numVertices()-1;
}

unsigned int TriMesh::addFace(unsigned int v1, unsigned int v2, unsigned int v3)
{
	Face f; f.v1 = v1; f.v2 = v2; f.v3 = v3;
	faces_.push_back( f );
	return numFaces()-1;
}

unsigned int TriMesh::addFace(const TriMesh::Face& f)
{
	faces_.push_back( f );
	return numFaces()-1;
}

} // End TinySG namespace
