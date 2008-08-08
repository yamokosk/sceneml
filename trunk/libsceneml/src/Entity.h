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

namespace tinysg {

// Forward declaration
class SceneObjectCollection;
class EntityFactory;
class Node;
class SceneManager;

//! Base class for movable scene objects.
/** @ingroup xode
 *  Base class for movalbe scene objects.
 */
class Entity
{
public:
	Entity();
	Entity(const std::string& name);
	virtual ~Entity();

	virtual Entity* clone() const = 0;

	void _notifyAttached(Node* parent);
	void _notifyCreator(EntityFactory* fact) { creator_ = fact; }
	void _notifyType(const std::string& type) { type_ = type; }
	virtual EntityFactory* _getCreator(void) const { return creator_; }
	virtual void _notifyManager(SceneManager* man) { manager_ = man; }
	virtual SceneManager* _getManager(void) const { return manager_; }

	//! Get obj name
	std::string getName() const {return name_;}
	//! Get obj type
	std::string getType() const {return type_;}

	virtual bool isAttached(void) const;

	virtual void _notifyMoved(void) = 0;
	//virtual void _notifySpace(Space* space) = 0;

	//virtual const AxisAlignedBox& getBoundingBox(void) const = 0;
	//virtual Real getBoundingRadius(void) const = 0;
	//virtual const AxisAlignedBox& getWorldBoundingBox(bool derive = false) const;
	//virtual const Sphere& getWorldBoundingSphere(bool derive = false) const;

	virtual void setVisible(bool visible) {visible_ = visible;}
	virtual bool getVisible(void) const {return visible_;}
	virtual bool isVisible(void) const { return visible_;}

protected:
	//! Is this object visible?
	bool visible_;
	//! Node to which this object is attached
	Node* 	parentNode_;
	//! SceneManager holding this object (if applicable)
	SceneManager* 	manager_;
	//! Objects name
	std::string name_;
	//! Object type
	std::string type_;
	//! Creator
	EntityFactory* creator_;
};


class EntityFactory
{
protected:
	virtual Entity* createInstanceImpl(const std::string& name, const PropertyCollection* params = 0) = 0;
public:
	EntityFactory();
	virtual ~EntityFactory();
	virtual std::string getType(void) const = 0;

	virtual Entity* createInstance( const std::string& name, SceneManager* manager, const PropertyCollection* params = 0);
	virtual void destroyInstance(Entity* obj) = 0;
};


} // Namespace

#endif
