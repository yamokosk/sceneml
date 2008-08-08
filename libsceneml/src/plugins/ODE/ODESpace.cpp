/*
 * Space.cpp
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#include "ODESpace.h"
#include "ODEGeom.h"

namespace smlode {

using namespace tinysg;

Space::Space() :
	Entity(),
	spaceID_(NULL)
{

}

Space::Space(const std::string& name) :
	Entity(name),
	spaceID_(NULL)
{

}

Space::~Space()
{
	if (spaceID_) dSpaceDestroy(spaceID_);
}

Entity* Space::clone() const
{
	return (new Space(*this));
}

void Space::addGeom(smlode::Geom* g)
{
	dSpaceAdd(spaceID_, g->_getGeomID());
}

inline
void Space::_notifyMoved(void)
{
}

SpaceFactory::SpaceFactory()
{

}

SpaceFactory::~SpaceFactory()
{

}

Entity* SpaceFactory::createInstanceImpl(const std::string& name, const PropertyCollection* params)
{
	Space* s = new Space( name );

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
		tinysg::Real depth = ExpressionFactory::getAsReal( params->getValue("depth") );

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
	}

	s->_setSpaceID(spaceID);
	return s;
}

std::string SpaceFactory::getType(void) const
{
	return std::string("ODE_SPACE");
}

void SpaceFactory::destroyInstance(Entity* obj)
{
	delete obj;
}

}
