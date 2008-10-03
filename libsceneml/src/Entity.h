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

#include <string>
#include <Exception.h>
#include <PropertyCollection.h>

namespace TinySG {

// Forward declaration
class SceneObjectCollection;
class EntityFactory;
class Node;
class SceneManager;

//! Base class for movable scene objects.
/** @ingroup xode
 *  Base class for movalbe scene objects.
 */
class Entity : public Object
{
public:
	Entity();
	virtual ~Entity();

	virtual Entity* clone() const = 0;

	void _notifyAttached(Node* parent);
	virtual bool isAttached(void) const;
	virtual void _notifyMoved(void) = 0;

	virtual void setVisible(bool visible) {visible_ = visible;}
	virtual bool getVisible(void) const {return visible_;}
	virtual bool isVisible(void) const { return visible_;}

protected:
	//! Is this object visible?
	bool visible_;
	//! Node to which this object is attached
	Node* 	parentNode_;
};


} // Namespace

#endif
