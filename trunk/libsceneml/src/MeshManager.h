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

#ifndef _MESHMGR_H_FILE_
#define _MESHMGR_H_FILE_


namespace TinySG {

// Forward declarations
class Mesh;

class MeshManager
{
public:
	// Entities
	typedef stdext::hash_map< std::string, Mesh*> MeshMap;
	typedef MeshMap::iterator MeshIterator;
	// Collection of one type of Mesh objects.. ODE, LINCANNY, etc.
	struct MeshCollection
	{
		MeshMap entities_;
	};
	typedef stdext::hash_map< std::string, MeshCollection*> MeshCollectionMap;

public:
	MeshManager();
	~MeshManager();

	// Entities
	Mesh* createMesh(const std::string& name,const std::string& typeName, const PropertyCollection* params = 0);
	void destroyMesh(const std::string& name, const std::string& typeName);
	void destroyMesh(Mesh* m);
	void destroyAllMeshesByType(const std::string& typeName);
	void destroyAllMeshes(void);
	Mesh* getMesh(const std::string& name, const std::string& typeName);
	MeshCollection* getEntitiesByType(const std::string& typeName);
	bool hasMesh(const std::string& name, const std::string& typeName) const;
	bool hasMesh(const Mesh* e) const;

private:
	MeshCollectionMap meshTypes_;
};


} // End TinySG namespace

#endif
