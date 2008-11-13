/*
 * DistanceQuery.cpp
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#include "DistanceQuery.h"
#include <ObjectManager.h>

namespace lincanny {

using namespace TinySG;


bool DistanceReport::inCollision()
{
	return (contactData_.size() > 0);
}

void DistanceReport::addContactPoint(const dContactGeom& data)
{
	contactData_.push_back( data );
}


//--------------------------------------------------------------------------------------
DistanceQuery::DistanceQuery() :
	TinySG::Query("LC_DISTANCE_QUERY")
{
}

DistanceQuery::~DistanceQuery()
{
}

QueryResult* DistanceQuery::execute(const TinySG::ObjectManager* mgr, const TinySG::PropertyCollection* params)
{
	DistanceReport* result = new DistanceReport(this);

	TinySG::ObjectManager::ObjectCollection* oc = mgr->getCollection( "LC_DISTANCE_PAIR" );

	TinySG::ObjectManager::ObjectsIterator iter = oc->objects.begin();
	TinySG::ObjectManager::ObjectsIterator iend = oc->objects.end();
	for(; iter != iend; ++iter)
	{
		sgode::CollisionPair* cp = static_cast<sgode::CollisionPair*>(iter->second);
		sgode::Space* s1 = dynamic_cast<sgode::Space*>( mgr->getObject(cp->first(), "LC_SPACE") );
		sgode::Space* s2 = dynamic_cast<sgode::Space*>( mgr->getObject(cp->second(), "LC_SPACE") );
		dSpaceCollide2( (dGeomID)s1->getOdeID(), (dGeomID)s2->getOdeID(), (void*)result, DistanceQuery::collisionCallback);
	}

	return result;
}

void DistanceQuery::collisionCallback(void* ptr, dGeomID o1, dGeomID o2)
{
	// TODO: If the Geom is a TriMesh, I might need to be setting the previous
	// transform with calls to void dGeomTriMeshSetLastTransform( dGeomID g, dMatrix4 last_trans )
	// and dReal* dGeomTriMeshGetLastTransform( dGeomID g )

	dContactGeom dContactPts[NUM_CONTACT_POINTS];

	int numContactPts = dCollide(o1, o2, NUM_CONTACT_POINTS, dContactPts, sizeof(dContactGeom));
	if (numContactPts > 0) {
		sgode::DistanceReport* res = static_cast<sgode::DistanceReport*>(ptr);
		for (int n=0; n < numContactPts; ++n) res->addContactPoint( dContactPts[n] );
	}
}




}
