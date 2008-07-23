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

#include <map>
#include <vector>
#include <list>
#include <sstream>

#include <Exception.h>

namespace sml {

//! Base class for movable scene objects.
/** @ingroup xode
 *  Base class for movalbe scene objects.
 */
class SceneObject
{
	friend class Scene;
public:
	// The following virutal functions must be implemented by child classes
	virtual ~SceneObjectBase() {};
	virtual ReturnMatrix getGlobalPosition() const = 0;
	virtual ReturnMatrix getGlobalRotation() const = 0;
	virtual ReturnMatrix getGlobalTransformation() const = 0;
	virtual void setGlobalPosition(const ColumnVector& pos) = 0;
	virtual void setGlobalRotation(const Matrix& rot) = 0;
	virtual void invalidate() = 0;
	virtual void validate() = 0;
	virtual unsigned getNumberOfProperties() const = 0;
	virtual int getProperty(const char* name, char* value) const = 0;
	virtual int setProperty(const char* name, const char* value) = 0;
	virtual bool hasProperty(const char* name) const = 0;
	virtual bool getPropertyName(unsigned idx, char* name) const = 0;
	//virtual int getPropertyReadOnly(const char* name, bool& readOnly) const = 0;
	//virtual int getPropertyInitStatus(const char* name, bool& preInit) const = 0;
	virtual unsigned getNumberOfPropertyValues(const char* propertyName) const = 0;
	virtual bool getPropertyValueAt(const char* propertyName, unsigned index, char* value) const = 0;

	ReturnMatrix getLocalPosition() const;
	ReturnMatrix getLocalRotation() const;
	ReturnMatrix getLocalTransformation() const;
	//! Adds a transform object.
	/** Notice that order which objects are added is extremely important. */
	//void addTransform(Transform *t) {transformList_.push_back(t);};
	void setCompositeTransform(CompositeTransformPtr t) {transform_ = t;};
	//! Get a pointer to the proximal object
	const SceneObjectBase* getProxObj() {return proxObj_;}
	//! Set the proximal object
	void setProxObj(SceneObjectBase* obj) {proxObj_ = obj;}
	//! Get wether the object's world pose is valid
	bool getPoseValidity() const {return validWorldPose_;}
	//! Get obj name
	std::string getName() const {return name_;}

protected:
	SceneObjectBase();
	virtual void computeLocalTransform();

	//! List of transform objects from the proximal object
	CompositeTransformPtr transform_;
	//! Objects name
	std::string name_;
	//! Hint to whether this object's world pose is valid
	bool validWorldPose_;
	//! Pointer to proximal object
	SceneObjectBase* proxObj_;
};

}; // Namespace

#endif
