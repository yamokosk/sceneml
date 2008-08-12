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


namespace TinySG {

// Forward declarations
class MeshFactory;


class Mesh
{
public:
	//! Constructor, only to be called by the creator MeshFactory.
	Mesh();
	Mesh(const std::string &name);

	//! Returns factory which created this mesh
	const MeshFactory* getCreator() const {return creator_;}

	//! Notify object of creator
	void _notifyCreator(MeshFactory* creator) {creator_ = creator;}
	//! Notify object of manager
	void _notifyManager(MeshManager* mgr) {mgr_ = mgr;}
	void _notifyType(const std::string& type) { type_ = type; }
	//! Get obj name
	const std::string& getName() const {return name_;}
	//! Get obj type
	const std::string& getType() const {return type_;}



protected:
	HardwareVertexBuffer* vertexBuffer_;
	HardwareIndexBuffer* indexBuffer_;

private:
	MeshFactory* creator_;
	MeshManager* mgr_;
	std::string name_;
	std::string type_;
};


class MeshFactory
{
protected:
	// To be overloaded by specific mesh factories
	virtual Mesh* createInstanceImpl(const std::string& name, const PropertyCollection* params = 0) = 0;

public:
	MeshFactory(const std::string& type);
	virtual ~MeshFactory();

	const std::string& getType(void) const;
	Mesh* createInstance( const std::string& name, SceneManager* manager, const PropertyCollection* params = 0);

	// To be overloaded by specific mesh factories
	virtual void destroyInstance(Entity* obj) = 0;

private:
	const std::string type_;
};

} // End TinySG namespace

#endif
