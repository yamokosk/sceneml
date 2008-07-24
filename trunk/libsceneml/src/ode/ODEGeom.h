/*
 * ODEGeom.h
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#ifndef ODEGEOM_H_
#define ODEGEOM_H_

#include "SceneObject.h"

namespace sml {

namespace ode {

class ODEGeom: public sml::SceneObject {
public:
	ODEGeom();
	ODEGeom(const std::string& name);
	virtual ~ODEGeom();

	void _setGeomID(dGeomID g) {geomID_ = g;}

	// Inherited from SceneObject
	virtual void _notifyMoved(void);
	//virtual void _notifySpace(Space* space);

private:
	//! ODE object pointer
	dGeomID geomID_;
	//! Geom mesh data
	PolyhedronPtr mesh_; // Owner of this pointer
	//! Geom color
	dVector3 rgb_;
	//! Geom alpha (transperancy)
	dReal alpha_;
};

class ODEObjectFactory
{
protected:
	virtual SceneObject* createInstanceImpl(const std::string& name, const PropertyCollection* params = 0) = 0;

public:
	ODEObjectFactory();
	virtual ~ODEObjectFactory();

	virtual const std::string& getType(void) const;
	virtual void destroyInstance(SceneObject* obj);
};

}

}

#endif /* ODEGEOM_H_ */
