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
/*
 * SceneObject.cpp
 *
 *  Created on: Jul 23, 2008
 *      Author: yamokosk
 */

#include "SceneObject.h"
#include "Node.h"

namespace sml
{

SceneObject::SceneObject() :
	visible_(true),
	parentNode_(NULL),
	manager_(NULL),
	creator_(NULL)
{
}

SceneObject::SceneObject(const std::string& name) :
	visible_(true),
	parentNode_(NULL),
	manager_(NULL),
	name_(name),
	creator_(NULL)
{
}

SceneObject::~SceneObject()
{
	if (parentNode_)
		parentNode_->detachObject(this);
}

void SceneObject::_notifyAttached(Node* parent)
{
	assert(!parentNode_ || !parent);

	bool different = (parent != parentNode_);

	parentNode_ = parent;
}

bool SceneObject::isAttached(void) const
{
	return (parentNode_ != 0);
}


SceneObject* SceneObjectFactory::createInstance(const std::string& name, SceneMgr* manager,
		const PropertyCollection* params)
{
	SceneObject* m = createInstanceImpl(name, params);
	m->_notifyCreator(this);
	m->_notifyManager(manager);
	return m;
}

} // Namespace sml
