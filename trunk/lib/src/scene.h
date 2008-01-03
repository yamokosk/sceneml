#ifndef XODE_SCENE_H_FILE
#define XODE_SCENE_H_FILE

 
#include <xode/matrix.h>
#include <xode/transform.h>
#include <xode/sceneobjects.h>

#include <string>
#include <map>
#include <list>
#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cassert>

#include <ode/ode.h> // ODE

namespace xode {

typedef std::map<std::string, dSpaceID> 	StringSpaceMap_t;
typedef std::pair<dSpaceID, dSpaceID> SpacePair_t;
typedef std::vector< SpacePair_t > SpacePairs_t;
typedef std::vector< dContactGeom > ContactGeoms_t;
typedef std::map<std::string, dReal*> StringVariableMap_t;
/**
 *  The scene class. A more elaborate scene description.
 */
class Scene
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
	GeomList_t getAllGeoms() const {return geomData_;};
	
	//Transform* createTransform();
	//Transform* getTransform(const std::string& varname);
	//CoordinateTransform* createTransform();
	//CoordinateTransform* getTransform(const std::string& varname);

	void addCollisionPair(const std::string& space1, const std::string& space2);
	ContactGeoms_t getContactData() const { return contactData_; };
	bool inCollision() const { return ((contactData_.size() > 0) ? true : false);}
		
	// Scene modification functions
	//void addVariable(const std::string& varname, const std::string& bodyname, Transform* transform);
	void addMutableValue(const std::string &name, dReal* v, Body* b);
	void setMutableValue(const std::string &name, dReal x, dReal y=0.0, dReal z=0.0);
	void update();
	
	// Miscellaneous functions
	void report(const std::string& name);
	void reportGeom(const std::string& name);
	void reportVars();
	
	//! Get ODE world pointer
	dWorldID getWorld() const {return worldID_;}
private:
	void collisionQuery();
	
	//! List of bodies
	BodyList_t		bodyData_;
	//! List of geoms
	GeomList_t		geomData_;
	
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

};


#endif