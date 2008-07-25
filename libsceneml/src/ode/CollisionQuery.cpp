/*
 * CollisionQuery.cpp
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#include "CollisionQuery.h"
#include <SceneMgr.h>

namespace sml {

namespace ode {

CollisionQuery::CollisionQuery(SceneMgr* mgr) :
	SceneQuery(mgr)
{
	// TODO Auto-generated constructor stub

}

CollisionQuery::~CollisionQuery() {
	// TODO Auto-generated destructor stub
}

void CollisionQuery::query()
{
	Space* space1 = manager_->getSceneObject(space1, "space");

	ODEGeom* g1; ODEGeom g2; int flags = 0; int skip = 0;
	dContactGeom contactGeom[MAX_NUM_CONTACT_PTS];
	int numContactPts = dCollide (g1->getGeomID(), g2->getGeomID(), flags, contactGeom, skip);
}


}

}
