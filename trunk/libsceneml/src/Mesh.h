/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
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

#ifndef _MESH_H_FILE_
#define _MESH_H_FILE_


namespace TinySG
{

class Mesh : public Object
{
public:
	Mesh() :
		vertex_count_(0), index_count_(0),
		vertex_stride_( 3*sizeof(float) ),
		index_stride_( sizeof(uint32_t) ),
		vertices_(NULL), indices_(NULL)
	{}
	virtual ~Mesh()
	{
		if (vertices_) delete [] vertices_;
		if (indices_) delete [] indices_;
	}

	uint32_t vertexCount() {return vertex_count_;}
	uint32_t indexCount() {return index_count_;}
	const float* vertexData() const { return vertices_; }
	const uint32_t* indexData() const { return indices_; }
	uint8_t vertexStride() {return vertex_stride_;}
	uint8_t indexStride() {return index_stride_;}

protected:
	uint32_t vertex_count_, index_count_;
	uint8_t vertex_stride_, index_stride_;
	float* vertices_;
	uint32_t* indices_;
};

} // End TinySG namespace

#endif
