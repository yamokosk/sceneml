/*
 * ODEGeom.h
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#ifndef ODEGEOM_H_
#define ODEGEOM_H_

// SceneML
#include <SceneML.h>

// ODE library
#include <ode/ode.h>

namespace smlode {

using namespace sml;

class Geom : public sml::Entity {
public:
	Geom();
	Geom(const std::string& name);
	virtual ~Geom();

	virtual Entity* clone() const;

	void _setGeomID(dGeomID g) {geomID_ = g;}
	dGeomID _getGeomID(void) {return geomID_;}
	int _getGeomClass() {return dGeomGetClass(geomID_);}

	// Inherited from Entity
	virtual void _notifyMoved(void);

private:
	//! ODE object pointer
	dGeomID geomID_;
	//! Geom mesh data
	//PolyhedronPtr mesh_; // Owner of this pointer
	//! Geom color
	ColumnVector rgb_;
	//! Geom alpha (transperancy)
	sml::Real alpha_;
};

class GeomFactory : public EntityFactory
{
protected:
	virtual Entity* createInstanceImpl(const std::string& name, const PropertyCollection* params = 0);

public:
	GeomFactory();
	virtual ~GeomFactory();

	virtual std::string getType(void) const;
	virtual void destroyInstance(Entity* obj);
};

}

#endif /* ODEGEOM_H_ */
