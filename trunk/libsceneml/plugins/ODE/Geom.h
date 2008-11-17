/*
 * ODEGeom.h
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#ifndef _ODE_GEOM_H_
#define _ODE_GEOM_H_

// TinySG
#include <TinySG.h>

// ODE library
#include <ode/ode.h>

// Logging
#include <log4cxx/logger.h>

namespace sgode {

using namespace TinySG;

class Geom : public TinySG::MovableObject
{
	friend class GeomFactory;
public:
	Geom();
	virtual ~Geom();

	// Inherited from TinySG::Object
	virtual Object* clone() const;
	void save(PropertyCollection& pc) const;

	dGeomID getOdeID(void) {return geomID_;}
	int getGeomClass() {return dGeomGetClass(geomID_);}

protected:
	// Inherited from Entity
	virtual void notifyMoved(void);

private:
	//! ODE object pointer
	dGeomID geomID_;
	//! Geom mesh data
	// Owner of the mesh data.. shares it with ODE
	TinySG::MeshPtr mesh_;
	//! Geom color
	TinySG::Real rgb_[3];
	//! Geom alpha (transperancy)
	TinySG::Real alpha_;
};


class GeomFactory : public ObjectFactory
{
	static log4cxx::LoggerPtr logger;
protected:
	// To be overloaded by specific object factories
	virtual Object* createInstanceImpl(const PropertyCollection* params = 0);

public:
	GeomFactory() : ObjectFactory("ODE_GEOM") {};
	virtual ~GeomFactory() {};

	// To be overloaded by specific object factories
	virtual void destroyInstance(Object* obj);
};

}

#endif /* ODEGEOM_H_ */
