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

namespace sml {

// Forward declaration
class SceneObjectFactory;

//! Base class for movable scene objects.
/** @ingroup xode
 *  Base class for movalbe scene objects.
 */
class SceneObject
{
public:
	SceneObject();
	SceneObject(const std::string& name);
	virtual ~SceneObjectBase();

	void _notifyAttached(Node* parent);
	void _notifyCreator(SceneObjectFactory* fact) { creator_ = fact; }
	virtual SceneObjectFactory* _getCreator(void) const { return creator_; }
	virtual void _notifyManager(SceneMgr* man) { manager_ = man; }
	virtual SceneMgr* _getManager(void) const { return manager_; }

	//! Get obj name
	std::string getName() const {return name_;}

	virtual bool isAttached(void) const;

	virtual void _notifyMoved(void) = 0;

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
	SceneMgr* 	manager_;
	//! Objects name
	std::string name_;
	//! Creator
	SceneObjectFactory* creator_;
};

class SceneObjectFactory
{
protected:
	//unsigned long mTypeFlag;

	virtual SceneObject* createInstanceImpl(const String& name, const NameValuePairList* params = 0) = 0;
public:
	SceneObjectFactory() /*: mTypeFlag(0xFFFFFFFF)*/ {}
	virtual ~SceneObjectFactory() {}
	virtual const std::string& getType(void) const = 0;

	virtual SceneObject* createInstance( const std::string& name, SceneMgr* manager, const PropertyCollection* params = 0);
	virtual void destroyInstance(SceneObject* obj) = 0;

	//virtual bool requestTypeFlags(void) const { return false; }
	//void _notifyTypeFlags(unsigned long flag) { mTypeFlag = flag; }

	//unsigned long getTypeFlags(void) const { return mTypeFlag; }
};


} // Namespace

#endif
