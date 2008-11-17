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

#ifndef _ODE_SPACE_H_
#define _ODE_SPACE_H_

// SceneML
#include <TinySG.h>

// ODE library
#include <ode/ode.h>

namespace sgode
{

using namespace TinySG;

// Forward declarations
class Geom;
class SpaceFactory;

class Space: public TinySG::Object
{
	friend class SpaceFactory;

public:
	Space();
	virtual ~Space();

	// From TinySG::Object
	virtual Object* clone() const;
	void save(PropertyCollection& pc) const;

	void addGeom(Geom* g);
	dSpaceID getOdeID() {return spaceID_;}
	int getGeomClass() {return dGeomGetClass((dGeomID)spaceID_);}

private:
	//! ODE object pointer
	dSpaceID spaceID_;
};


class SpaceFactory : public ObjectFactory
{
protected:
	// To be overloaded by specific object factories
	virtual Object* createInstanceImpl(const PropertyCollection* params = 0);

public:
	SpaceFactory() : ObjectFactory("ODE_SPACE") {};
	virtual ~SpaceFactory() {};

	// To be overloaded by specific object factories
	virtual void destroyInstance(Object* obj);
};


class CollisionPair : public TinySG::Object
{
public:
	CollisionPair(const std::string& one, const std::string& two) : TinySG::Object(), space1_(one), space2_(two) {}
	virtual ~CollisionPair() {}

	// Inherited from TinySG::Object
	virtual Object* clone() const;
	void save(PropertyCollection& pc) const;

	const std::string& first() const {return space1_;}
	const std::string& second() const {return space2_;}

private:
	std::string space1_;
	std::string space2_;
};


class CollisionPairFactory : public TinySG::ObjectFactory
{
protected:
	// To be overloaded by specific object factories
	virtual Object* createInstanceImpl(const PropertyCollection* params = 0);

public:
	CollisionPairFactory() : ObjectFactory("ODE_COLLISION_PAIR") {};
	virtual ~CollisionPairFactory() {};

	// To be overloaded by specific object factories
	virtual void destroyInstance(Object* obj);
};

}

#endif /* ODESPACE_H_ */
