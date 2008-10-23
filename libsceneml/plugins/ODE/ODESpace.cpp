/*
 * Space.cpp
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#include "ODESpace.h"
#include "ODEGeom.h"

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
		ColumnVector center = ExpressionFactory::getAsVector( params->getValue("center"), 3 );
		ColumnVector extents = ExpressionFactory::getAsVector( params->getValue("extents"), 3 );
		TinySG::Real depth = ExpressionFactory::getAsReal( params->getValue("depth") );

		dVector3 dCenter = {0}, dExtents = {0};
		for (int n=0; n<3; ++n) {
			dCenter[n] = (dReal)center(n+1);
			dExtents[n] = (dReal)extents(n+1);
		}

		spaceID = dQuadTreeSpaceCreate(parentID, dCenter, dExtents, (int)depth);
	} else if (spaceType.compare("quadtree") == 0) {
		spaceID = dHashSpaceCreate(parentID);

		float minlevel = ExpressionFactory::getAsReal( params->getValue("minlevel") );
		float maxlevel = ExpressionFactory::getAsReal( params->getValue("maxlevel") );
		dHashSpaceSetLevels(spaceID, (int)minlevel, (int)maxlevel);
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
