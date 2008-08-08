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
/*
 * ODESpace.h
 *
 *  Created on: Jul 30, 2008
 *      Author: yamokosk
 */

#ifndef ODESPACE_H_
#define ODESPACE_H_

// SceneML
#include <SceneML.h>

// ODE library
#include <ode/ode.h>

namespace smlode
{

using namespace tinysg;

// Forward declarations
class Geom;

class Space: public tinysg::Entity
{
public:
	Space();
	Space(const std::string& name);
	virtual ~Space();

	virtual Entity* clone() const;

	void _setSpaceID(dSpaceID s) {spaceID_ = s;}
	dSpaceID _getGeomID(void) {return spaceID_;}
	void addGeom(smlode::Geom* g);

	// Inherited from Entity
	virtual void _notifyMoved(void);

private:
	//! ODE object pointer
	dSpaceID spaceID_;
};

class SpaceFactory : public EntityFactory
{
protected:
	virtual Entity* createInstanceImpl(const std::string& name, const PropertyCollection* params = 0);

public:
	SpaceFactory();
	virtual ~SpaceFactory();

	virtual std::string getType(void) const;
	virtual void destroyInstance(Entity* obj);
};

}

#endif /* ODESPACE_H_ */
