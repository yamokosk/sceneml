/*
 * CollisionQuery.cpp
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#include "CollisionQuery.h"
#include <SceneMgr.h>

namespace smlode {

using namespace sml;

CollisionQuery::CollisionQuery(SceneMgr* mgr) :
	SceneQuery(mgr)
{
	// TODO Auto-generated constructor stub

}

CollisionQuery::~CollisionQuery() {
	// TODO Auto-generated destructor stub
}

SceneQueryResult* CollisionQuery::query()
{
	/*Space* space1 = manager_->getSceneObject(space1, "space");

	Geom* g1; Geom g2; int flags = 0; int skip = 0;
	dContactGeom contactGeom[MAX_NUM_CONTACT_PTS];
	int numContactPts = dCollide (g1->getGeomID(), g2->getGeomID(), flags, contactGeom, skip);*/

	return NULL;
}


}