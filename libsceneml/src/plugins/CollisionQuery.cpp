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

CollisionQuery::CollisionQuery(SceneManager* mgr) :
	SceneQuery(mgr),
	inCollision_(false)
{
	// TODO Auto-generated constructor stub

}

CollisionQuery::~CollisionQuery()
{

}

virtual SceneQuery* CollisionQuery::clone() const
{
	return (new CollisionQuery(*this));
}

virtual void CollisionQuery::execute(const SceneManager* mgr)
{
	this->notify(SceneQuery::QUERY_STARTED);

	SceneManager::EntityPairsIterator itr = mgr->getEntityPairsIterator();

	// Reset contactData vector
	//contactData_.clear();

	// Now perform space to space collision checks
	//for (unsigned int n=0; n < spacePairs_.size(); ++n)
	//		dSpaceCollide2((dGeomID)spacePairs_[n].first, (dGeomID)spacePairs_[n].second, (void*)&contactData_, collisionCallback);

	this->notify(SceneQuery::QUERY_COMPLETE);
}

virtual std::string CollisionQuery::getType() const
{
	return "ODE_Collision_Check";
}

virtual QueryResult* getResult()
{
	SimpleResult* result(this);
	result->_setCollisionStatus(inCollision_);
	return result;
}

virtual void deleteResult( QueryResult* result )
{
	delete result;
}


SimpleResult::SimpleResult(sml::SceneQuery* creator) :
	sml::QueryResult(creator),
	inCollision(true)
{

}

SimpleResult::~SimpleResult()
{

}

bool SimpleResult::inCollision()
{
	return inCollision_;
}

void SimpleResult::_setCollisionStatus(bool status)
{
	inCollision_ = status;
}

void collisionCallback(void* data, dGeomID o1, dGeomID o2)
{
	// TODO: If the Geom is a TriMesh, I might need to be setting the previous
	// transform with calls to void dGeomTriMeshSetLastTransform( dGeomID g, dMatrix4 last_trans )
	// and dReal* dGeomTriMeshGetLastTransform( dGeomID g )
	//bool* bVal = (bool*)data;

	unsigned long col1 = dGeomGetCollideBits(o1);
	unsigned long col2 = dGeomGetCollideBits(o2);

	int flags = 0;
	//mBitsOn(flags, NUM_CONTACT_POINTS);
	dContactGeom dContactPts[NUM_CONTACT_POINTS];

	int numContactPts = dCollide(o1, o2, flags, dContactPts, sizeof(dContactGeom));
	if (numContactPts > 0) {
		ContactGeoms_t* pContactData = (ContactGeoms_t*)data;
		for (int n=0; n < numContactPts; ++n) (*pContactData).push_back( dContactPts[n] );
	}
}

}
