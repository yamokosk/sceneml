/*
 * Space.cpp
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#include "Space.h"
#include "RigidBodyAdapter.h"

namespace lincanny
{

using namespace TinySG;

Space::Space() :
	Object()
{

}

Space::~Space()
{
}

Object* Space::clone() const
{
	return (new Space(*this));
}

void Space::addBody(RigidBodyAdapter* b)
{
	bodies_.push_back(b);
}

RigidBodyAdapter* Space::getBody(size_t n)
{
	if ( n < getNumBodies() )
	{
		return bodies_[n];
	} else {
		// Throw an error.
	}
}

//---------------------------------------------------------------------------------
Object* SpaceFactory::createInstanceImpl(const PropertyCollection* params)
{
	Space* s = new Space();
	return s;
}

void SpaceFactory::destroyInstance(Object* obj)
{
	Space* s = dynamic_cast<Space*>(obj);
	delete s;
}


//---------------------------------------------------------------------------------
Object* CollisionPair::clone() const
{
	return (new CollisionPair(*this));
}


//---------------------------------------------------------------------------------
Object* CollisionPairFactory::createInstanceImpl(const PropertyCollection* params)
{
	if ( params == NULL )
	{
		SML_EXCEPT(TinySG::Exception::ERR_INVALIDPARAMS, "Collision pair parameters were not specified.");
	}

	return new CollisionPair(params->getValue("space1"), params->getValue("space2"));
}

void CollisionPairFactory::destroyInstance(Object* obj)
{
	CollisionPair* s = dynamic_cast<CollisionPair*>(obj);
	delete s;
}
}
