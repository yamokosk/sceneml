/*
 * CollisionQuery.cpp
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#include "CollisionQuery.h"
#include <ObjectManager.h>

namespace sgode {

using namespace TinySG;


bool CollisionReport::inCollision()
{
	return (contactData_.size() > 0);
}

void CollisionReport::addContactPoint(const dContactGeom& data)
{
	contactData_.push_back( data );
}


//--------------------------------------------------------------------------------------
CollisionQuery::CollisionQuery() :
	TinySG::Query("ODE_COLLISION_CHECK")
{
}

CollisionQuery::~CollisionQuery()
{
}

QueryResult* CollisionQuery::execute(const TinySG::ObjectManager* mgr, const TinySG::PropertyCollection* params)
{
	CollisionReport* result = new CollisionReport(this);

	TinySG::ObjectManager::ObjectCollection* oc = mgr->getCollection( "ODE_COLLISION_PAIR" );

	TinySG::ObjectManager::ObjectsIterator iter = oc->objects.begin();
	TinySG::ObjectManager::ObjectsIterator iend = oc->objects.end();
	for(; iter != iend; ++iter)
	{
		sgode::CollisionPair* cp = static_cast<sgode::CollisionPair*>(iter->second);
		sgode::Space* s1 = dynamic_cast<sgode::Space*>( mgr->getObject(cp->first(), "ODE_SPACE") );
		sgode::Space* s2 = dynamic_cast<sgode::Space*>( mgr->getObject(cp->second(), "ODE_SPACE") );
		dSpaceCollide2( (dGeomID)s1->getOdeID(), (dGeomID)s2->getOdeID(), (void*)result, CollisionQuery::collisionCallback);
	}

	return result;
}

void CollisionQuery::collisionCallback(void* ptr, dGeomID o1, dGeomID o2)
{
	// TODO: If the Geom is a TriMesh, I might need to be setting the previous
	// transform with calls to void dGeomTriMeshSetLastTransform( dGeomID g, dMatrix4 last_trans )
	// and dReal* dGeomTriMeshGetLastTransform( dGeomID g )

	dContactGeom dContactPts[NUM_CONTACT_POINTS];

	int numContactPts = dCollide(o1, o2, NUM_CONTACT_POINTS, dContactPts, sizeof(dContactGeom));
	if (numContactPts > 0) {
		sgode::CollisionReport* res = static_cast<sgode::CollisionReport*>(ptr);
		for (int n=0; n < numContactPts; ++n) res->addContactPoint( dContactPts[n] );
	}
}




}
