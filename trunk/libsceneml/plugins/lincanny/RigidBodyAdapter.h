/*
 * ODEGeom.h
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#ifndef ODEGEOM_H_
#define ODEGEOM_H_

// TinySG
#include <TinySG.h>

// ODE library
#include "RigidBody.h"

// Logging
#include <log4cxx/logger.h>

namespace lincanny {

using namespace TinySG;

class RigidBodyAdapter : public TinySG::MovableObject
{
	friend class RigidBodyAdapterFactory;
public:
	RigidBodyAdapter();
	virtual ~RigidBodyAdapter();

	virtual Object* clone() const;

	RigidBody* getPtr(void) {return body_;}

protected:
	// Inherited from Entity
	virtual void notifyMoved(void);

private:
	//! ODE object pointer
	RigidBody* body_;
	//! Geom mesh data
	//PolyhedronPtr mesh_; // Owner of this pointer
	//! Geom color
	TinySG::Real rgb_[3];
	//! Geom alpha (transperancy)
	TinySG::Real alpha_;
};


class RigidBodyAdapterFactory : public ObjectFactory
{
	static log4cxx::LoggerPtr logger;
protected:
	// To be overloaded by specific object factories
	virtual Object* createInstanceImpl(const PropertyCollection* params = 0);

public:
	RigidBodyAdapterFactory() : ObjectFactory("LC_BODY") {};
	virtual ~RigidBodyAdapterFactory() {};

	// To be overloaded by specific object factories
	virtual void destroyInstance(Object* obj);
};

}

#endif /* ODEGEOM_H_ */
