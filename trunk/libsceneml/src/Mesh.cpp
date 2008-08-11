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

#include "Mesh.h"

namespace TinySG
{

Mesh::Mesh() :
	name_("")
{

}

Mesh::Mesh(const std::string &name) :
	name_(name)
{

}

MeshFactory::MeshFactory()
{}

MeshFactory::~MeshFactory()
{}

Mesh* MeshFactory::createInstance(const std::string& name, MeshManager* manager, const PropertyCollection* params)
{
	Mesh* m = createInstanceImpl(name, params);
	m->_notifyCreator(this);
	m->_notifyManager(manager);
	return m;
}

} // End TinySG namespace
