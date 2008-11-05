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

#ifndef _SCENE_OBJ_H_FILE_
#define _SCENE_OBJ_H_FILE_

// External includes
#include <string>

// Local includes
#include "Exception.h"
#include "Object.h"
#include "PropertyCollection.h"

namespace TinySG {

class Node;

//! Base class for movable scene objects.
/** @ingroup xode
 *  Base class for movalbe scene objects.
 */
class MovableObject : public Object
{
	friend class Node;

public:
	MovableObject();
	virtual ~MovableObject();

	virtual Object* clone() const = 0;

	virtual void setVisible(bool visible) {visible_ = visible;}
	virtual bool isVisible(void) const { return visible_;}

	virtual bool isAttached(void) const;

protected:
	void notifyAttached(Node* parent);
	virtual void notifyMoved(void) = 0;

	//! Is this object visible?
	bool visible_;
	//! Node to which this object is attached
	Node* parentNode_;
};


/*
class MovableObjectFactory : public ObjectFactory
{
protected:
	// To be overloaded by specific object factories
	virtual Object* createInstanceImpl(const PropertyCollection* params = 0);

public:
	MovableObjectFactory(const std::string& type) : ObjectFactory(type) {};
	virtual ~MovableObjectFactory() {};

	// To be overloaded by specific object factories
	virtual void destroyInstance(Object* obj);
};*/

} // Namespace

#endif
