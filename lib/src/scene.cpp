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

#include "scene.h"
#include "matrix.h"

using namespace sceneml;

void collisionCallback(void* data, dGeomID o1, dGeomID o2);

//! Basic constructor.
Scene::Scene() :
	bCollisionState_(true)
{
	dInitODE();
	worldID_ = dWorldCreate();
	
	// Create a world body
	Body* world = createBody("world", NULL);
	CompositeTransform *pTransform = new CompositeTransform();
	world->setCompositeTransform(pTransform);
	world->validate();
}

//! Basic destructor
Scene::~Scene()
{
	for (BodyList_t::iterator it = bodyData_.begin(); it != bodyData_.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	bodyData_.clear();
	
	for (GeomList_t::iterator it = geomData_.begin(); it != geomData_.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	geomData_.clear();
		
	dWorldDestroy(worldID_);
	dCloseODE();
}

//! Gets an ODE space ID by name.
dSpaceID Scene::getSpace( const std::string& name )
{
	StringSpaceMap_t::iterator it = spaceMap_.find(name);
	if (it != spaceMap_.end()) {
		return it->second;
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Space " + name + " does not exist in the scene.";
		throw std::runtime_error(msg.str());
	}
}

//! Adds an ODE body
Body* Scene::createBody(const std::string& name, dBodyID id)
{
	StringBodyMap_t::iterator it = bodyMap_.find(name);
	
	if (it == bodyMap_.end()) {
		Body* body = new Body(id);
		body->name_ = name;
		bodyMap_[name] = body;
		bodyData_.push_back(body);
		return body;
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Body " + name + " already exists in the scene.";
		throw std::runtime_error(msg.str());
	}
}

//! Gets a body object by name
Body* Scene::getBody(const std::string& name)
{
	StringBodyMap_t::iterator it = bodyMap_.find(name);
	
	if (it != bodyMap_.end()) {
		return it->second;
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Body " + name + " does not exist in the scene.";
		throw std::runtime_error(msg.str());
	}
}

//! Adds an ODE geom
Geom* Scene::createGeom(const std::string& name, dGeomID id, dGeomID t)
{
	StringGeomMap_t::iterator it = geomMap_.find(name);
	
	if (it == geomMap_.end()) {
		Geom* geom = new Geom(id, t);
		geom->name_ = name;
		geomMap_[name] = geom;
		geomData_.push_back(geom);
		return geom;
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Geom " + name + " already exists in the scene.";
		throw std::runtime_error(msg.str());
	}
}

//! Gets a geom object by name
Geom* Scene::getGeom(const std::string& name)
{
	StringGeomMap_t::iterator it = geomMap_.find(name);
	
	if (it != geomMap_.end()) {
		return it->second;
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Geom " + name + " does not exist in the scene.";
		throw std::runtime_error(msg.str());
	}
}

std::string Scene::getGeomByID(const dGeomID id)
{
	StringGeomMap_t::iterator it = geomMap_.begin();
	Geom* xodeid = NULL;
	
	for (; it != geomMap_.end(); ++it) {
		xodeid = it->second;
		if (xodeid->id() == id) return it->first;
	}
	
	return std::string("NO_DATA");
}
	
//! Create a transform object
//Transform* Scene::createTransform()
/*CoordinateTransform* Scene::createTransform()
{
	Transform *transform = new Transform;
	
	transformData_.push_back(transform);
	
	return transform;
}*/

//Transform* Scene::getTransform(const std::string& varname)
/*CoordinateTransform* Scene::getTransform(const std::string& varname)
{
	StringTransformMap_t::iterator it = variableMap_.find(varname);
	
	if ( it != variableMap_.end() )
		return it->second;
	else 
		return NULL;
}*/

void Scene::addMutableValue(const std::string &name, dReal* v, Body* b)
{
	StringVariableMap_t::iterator it = variableMap_.find(name);
	if (it == variableMap_.end())
		variableMap_[name] = v;
	else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Variable " << name << " is already registered as a mutable variable in the scene.";
		throw std::runtime_error(msg.str());
	}
	
	StringBodyMap_t::iterator bt = varBodyMap_.find(name);
	if (bt == varBodyMap_.end())
		varBodyMap_[name] = b;
	else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Variable " << name << " is already associated with body " << b->getName() << ".";
		throw std::runtime_error(msg.str());
	}
}

void Scene::setMutableValue(const std::string &name, dReal x, dReal y, dReal z)
{
	StringVariableMap_t::iterator it = variableMap_.find(name);
	if (it != variableMap_.end()) {
		(it->second)[0] = x;
		(it->second)[1] = y;
		(it->second)[2] = z;
	} else {
		std::cerr << __FUNCTION__ << "(): Mutable value " << name << " does not exist! Skipping it." << std::endl;
		return;
	}

	StringBodyMap_t::iterator vb = varBodyMap_.find(name);
	if (vb != varBodyMap_.end()) 
	{		
		if (vb->second != NULL) (vb->second)->invalidate();
		else {
			std::cerr << __FUNCTION__ << "(): Body pointer associated with variable " 
					<< name << " was NULL! How could this be?" << std::endl;
			return;
		}
	} else {
		std::cerr << __FUNCTION__ << "(): Could not find a body associated with variable " 
					<< name << std::endl;
		return;
	}
}

/*void Scene::addVariable(Transform* t, Body* b)
{
	StringTransformMap_t::iterator it = variableMap_.find(t->getName());
	if (it == variableMap_.end())
		variableMap_[t->getName()] = t;
	else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Variable " << t->getName() << " already exists in the scene.";
		throw std::runtime_error(msg.str());
	}
	
	try {
		varBodyMap_[t->getName()] = b;
	} catch (std::runtime_error& e) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): An error occured while attempting to associate variable " 
						<< t->getName() << " with body " << b->getName() << ":" << std::endl << e.what();
		throw std::runtime_error(msg.str());
	} catch (...) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): An unknown error occured while attempting to associate variable " 
						<< t->getName() << " with body " << b->getName() << ".";
		throw std::runtime_error(msg.str());
	}
}*/

void Scene::update()
{
	//std::cout << "In update()" << std::endl;
	for (BodyList_t::iterator it = bodyData_.begin(); it != bodyData_.end(); ++it)
	{
		(*it)->validate();
	}
		
	for (GeomList_t::iterator it = geomData_.begin(); it != geomData_.end(); ++it)
	{
		(*it)->validate();
		dGeomTriMeshClearTCCache((*it)->id()); // Is this necessary?
	}
	//std::cout << "Finished update(), going into collision query" << std::endl;
	this->collisionQuery();
}

/*void Scene::setMutableVars(ValueList_t& varlist)
{	
	BodyList_t distBodyToVisit, newset;
	BodyList_t::iterator distBodyIterator;

	ValueList_t::iterator it;
	for (it = varlist.begin(); it != varlist.end(); it++) 
	{		
		// Get the associated transform so we can update its value
		Transform* transform = getTransform((*it).name_);
		
		if (transform != NULL) {
			//for (int n=0; n < 3; ++n) transform->value_[n] = (*it).value_[n];						 
			memcpy(transform->value_, (*it).value_, sizeof(float)*3);
		} else {
			std::cerr << "Transform with variable " << (*it).name_ << " does not exist! Skipping it." << std::endl;
			continue;
		}
	
		// Get the associated body so we can invalidate its world pose
		StringBodyMap_t::iterator vb = varBodyMap_.find((*it).name_);
		Body *body = NULL;
		if (vb != varBodyMap_.end()) {		
			body = vb->second;
			if (body != NULL) {
				body->invalidate();
			} else {
				std::cerr << "Scene::setMutableVars() - Could not find a body associated with variable " 
						<< (*it).name_ << std::endl;
				continue;
			}
		} else {
			std::cerr << "Scene::setMutableVars() - Could not find a body associated with variable " 
						<< (*it).name_ << std::endl;
			continue;
		}
	}
}*/

void Scene::addCollisionPair(const std::string& space1, const std::string& space2)
{
	try {
		dSpaceID id1 = getSpace( space1 );
		dSpaceID id2 = getSpace( space2 );
		
		spacePairs_.push_back( SpacePair_t(id1,id2) );
	} catch (std::runtime_error& e) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): space1 = " << space1 << " space2 = " << space2 << std::endl << e.what();
		throw std::runtime_error(msg.str());
	}
}

void collisionCallback(void* data, dGeomID o1, dGeomID o2)
{
	// TODO: If the Geom is a TriMesh, I might need to be setting the previous
	// transform with calls to void dGeomTriMeshSetLastTransform( dGeomID g, dMatrix4 last_trans )
	// and dReal* dGeomTriMeshGetLastTransform( dGeomID g )
	//bool* bVal = (bool*)data;
		
	int flags = 0;
	mBitsOn(flags, NUM_CONTACT_POINTS);
	dContactGeom dContactPts[NUM_CONTACT_POINTS];
	
	int numContactPts = dCollide(o1, o2, flags, dContactPts, sizeof(dContactGeom));
	if (numContactPts > 0) {
		ContactGeoms_t* pContactData = (ContactGeoms_t*)data;
		for (int n=0; n < numContactPts; ++n) (*pContactData).push_back( dContactPts[n] );
	}
}

void Scene::collisionQuery()
{
	int nChecks = spacePairs_.size();
	
	// Reset contactData vector
	contactData_.clear();
	
	// Now perform space to space collision checks
	for (unsigned int n=0; n < spacePairs_.size(); ++n)
		dSpaceCollide2((dGeomID)spacePairs_[n].first, (dGeomID)spacePairs_[n].second, (void*)&contactData_, collisionCallback);
}

void Scene::report(const std::string& name)
{
	Body* body = getBody(name);
	
	if (body != NULL)
	{
		dMatrix4 T;
		body->getLocalTransformation(T);
		std::cout << "Local Transformation:" << std::endl;
		dTPrint(T);
		
		body->getGlobalTransformation(T);
		std::cout << "Global Transformation:" << std::endl;
		dTPrint(T);
	}
}

void Scene::reportGeom(const std::string& name)
{
	Geom* geom = getGeom(name);
	
	if (geom != NULL)
	{
		dMatrix4 T;
		geom->getLocalTransformation(T);
		std::cout << "Local Transformation:" << std::endl;
		dTPrint(T);
		
		geom->getGlobalTransformation(T);
		std::cout << "Global Transformation: " << std::endl;
		dTPrint(T);
	}
}

void Scene::reportVars()
{
	StringBodyMap_t::iterator it = varBodyMap_.begin();
	
	for (; it != varBodyMap_.end(); ++it)
	{
		std::cout << "Var: " << it->first << ", Body: " << (it->second)->getName() << std::endl;
	}
}
