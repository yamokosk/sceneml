/*
 * Space.cpp
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#include "Space.h"
#include "Geom.h"

#include <tinysg/MathExpression.h>
#include <tinysg/Vector3.h>

namespace sgode
{

using namespace TinySG;

Space::Space() :
	Object(),
	spaceID_(NULL)
{

}

Space::~Space()
{
	if (spaceID_ != NULL) dSpaceDestroy(spaceID_);
}

Object* Space::clone() const
{
	return (new Space(*this));
}

void Space::save(PropertyCollection& pc) const
{
	// Class identifier
	pc.addPair( RequiredProperty("class", "space") );

	// Add properties used to create this space.
	pc.combine( this->getProperties() );
}

void Space::addGeom(Geom* g)
{
	dSpaceAdd(getOdeID(), g->getOdeID());
}


//---------------------------------------------------------------------------------
Object* SpaceFactory::createInstanceImpl(const PropertyCollection* params)
{
	if ( params == NULL )
	{
		SML_EXCEPT(TinySG::Exception::ERR_INVALIDPARAMS, "Space parameters were not specified.");
	}

	Space* s = new Space();

	dSpaceID spaceID = NULL, parentID = NULL;

	// Get parent if one exists
	/*std::string spaceParent = params->getValue("parent");
	if ( spaceParent.compare("_NOPARENT_") != 0 ) {
		parentID = scene_->getSpace( spaceParent );
	}*/

	// Get the type now so we build the correct type
	std::string spaceType = params->getValue("type");
	if ( spaceType.compare("simple") == 0 ) {
		spaceID = dSimpleSpaceCreate(parentID);
	} else if (spaceType.compare("hash") == 0) {
		Vector3 center = ExpressionFactory::getAsSequence<Vector3>(params->getValue("center"), 3);
		Vector3 extents = ExpressionFactory::getAsSequence<Vector3>( params->getValue("extents"), 3 );
		TinySG::Real depth = ExpressionFactory::getAsReal( params->getValue("depth") );

		dVector3 dCenter = {0}, dExtents = {0};
		for (int n=0; n<3; ++n) {
			dCenter[n] = (dReal)center[n];
			dExtents[n] = (dReal)extents[n];
		}

		spaceID = dQuadTreeSpaceCreate(parentID, dCenter, dExtents, (int)depth);
	} else if (spaceType.compare("quadtree") == 0) {
		spaceID = dHashSpaceCreate(parentID);

		int minlevel = ExpressionFactory::getAsInt(params->getValue("minlevel"));
		int maxlevel = ExpressionFactory::getAsInt(params->getValue("maxlevel"));
		dHashSpaceSetLevels(spaceID, minlevel, maxlevel);
	} else {
		SML_EXCEPT(TinySG::Exception::ERR_INVALIDPARAMS, "Space type " + spaceType + " is an unknown type.");
	}

	s->spaceID_ = spaceID;
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

void CollisionPair::save(PropertyCollection& pc) const
{
	// Class identifier
	pc.addPair( RequiredProperty("class", "collisionpair") );

	// Add properties used to create this space.
	pc.combine( this->getProperties() );
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
