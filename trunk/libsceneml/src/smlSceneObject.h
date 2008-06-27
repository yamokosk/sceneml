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

#include <ode/ode.h>

#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <stdexcept>

#include "config.h"
#include "transform.h"
#include "Primitive.h"

#include <boost/shared_ptr.hpp>

namespace sceneml {

class SCENEML_API Geom;
class SCENEML_API Body;

typedef boost::shared_ptr<Geom> GeomPtr;
typedef boost::shared_ptr<Body> BodyPtr;

typedef std::vector< Body* > BodyList_t;
typedef std::vector< Geom* > GeomList_t;

typedef std::list< BodyPtr > BodyPtrList_t;
typedef std::list< GeomPtr > GeomPtrList_t;

typedef std::map<std::string, Body*> StringBodyMap_t;
typedef std::map<std::string, Geom*> StringGeomMap_t;

//! Base class for movable scene objects.
/** @ingroup xode 
 *  Base class for movalbe scene objects.
 */
class SceneObject : public Listener
{
	friend class Scene;
public:
	// The following virutal functions must be implemented by child classes
	virtual ~SceneObjectBase() {};
	
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
	
	//! From Listener
	virtual void notify(bool bExtended) const = 0;
	
	
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

	//! Is this object visible? 
	bool visible_;
	
	//! Node to which this object is attached
	SceneNode* 	parentNode_;
	
	//! SceneManager holding this object (if applicable)
	SceneMgr* 	manager_;
	
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
