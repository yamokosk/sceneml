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

#ifndef SCENEML_SCENE_H_FILE
#define SCENEML_SCENE_H_FILE

#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <stdexcept>

#include <ode/ode.h>

#include "config.h"
#include "sceneobjects.h"
#include "transform.h"

#include <boost/shared_ptr.hpp>

#define NUM_CONTACT_POINTS 1

namespace sceneml {

typedef std::map<std::string, dSpaceID> 	StringSpaceMap_t;
typedef std::pair<dSpaceID, dSpaceID> SpacePair_t;
typedef std::vector< SpacePair_t > SpacePairs_t;
typedef std::vector< dContactGeom > ContactGeoms_t;
typedef std::map<std::string, dRealPtr> StringVariableMap_t;

/**
 *  The scene class. A more elaborate scene description.
 */
class SCENEML_API Scene
{	
public:
	Scene();
	~Scene();
	
	//! Add an ODE space
	void addSpace( const std::string& name, dSpaceID id ) {spaceMap_[name] = id;}
	dSpaceID getSpace( const std::string& name );
	
	Body* createBody(const std::string& name, dBodyID id);
	Body* getBody(const std::string& name);
	
	Geom* createGeom(const std::string& name, dGeomID id, dGeomID t=NULL);
	Geom* getGeom(const std::string& name);
	
	std::string getGeomByID(const dGeomID id);
	std::list< boost::shared_ptr<Geom> > getAllGeoms() const {return geomData_;};
	
	void addCollisionPair(const std::string& space1, const std::string& space2);
	ContactGeoms_t getContactData() const { return contactData_; };
	bool inCollision() const { return ((contactData_.size() > 0) ? true : false);}
		
	// Scene modification functions
	//void addVariable(const std::string& varname, const std::string& bodyname, Transform* transform);
	void addMutableValue(const std::string &name, dRealPtr v, Body* b);
	void setMutableValue(const std::string &name, dReal x, dReal y=0.0, dReal z=0.0);
	void getMutableValue(const std::string &name, dReal& x, dReal& y, dReal& z);
	std::list< std::string > getVarNames();
	void update();
	
	// Miscellaneous functions
	void report(const std::string& name);
	void reportGeom(const std::string& name);
	void reportVars();
	
	//! Get ODE world pointer
	dWorldID getWorld() const {return worldID_;}
	
	void computeSceneAABB(dReal aabb[6]);

private:
	void collisionQuery();
	
	//! List of bodies
	BodyPtrList_t bodyData_;
	
	//! List of geoms
	GeomPtrList_t geomData_;
	
	//! Space name to dSpaceID map
	StringSpaceMap_t		spaceMap_;
	//! Geom name to geom pointer map
	StringGeomMap_t		geomMap_;
	//! Body name to body pointer map
	StringBodyMap_t		bodyMap_;
	//! Variable name to body pointer map
	StringBodyMap_t		varBodyMap_;
	//! Variable name to transform pointer map
	StringVariableMap_t		variableMap_;	
	//! Current contact data
	ContactGeoms_t			contactData_;
	
	//! Space pairs
	SpacePairs_t spacePairs_;
	
	//! Collision state of the scene
	bool bCollisionState_;
	
	//! The world.. according to ODE
	dWorldID worldID_;
};

typedef std::auto_ptr<Scene> ScenePtr;

};


#endif
