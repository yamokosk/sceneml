/*
 * CollisionQuery.h
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#ifndef COLLISIONQUERY_H_
#define COLLISIONQUERY_H_

// SceneML
#include <TinySG.h>

// ODE library
#include <ode/ode.h>

#include "ODEGeom.h"
#include "ODESpace.h"

#define NUM_CONTACT_POINTS 1

namespace sgode {


class CollisionReport : public TinySG::QueryResult
{
	typedef std::vector<dContactGeom> ContactGeometries;
public:
	CollisionReport(TinySG::Query* creator) : TinySG::QueryResult(creator) {};
	virtual ~CollisionReport() {};

	bool inCollision();
	void addContactPoint(const dContactGeom&);
	ContactGeometries getContactData() {return contactData_;}

private:
	ContactGeometries contactData_;
};

typedef auto_ptr<CollisionReport> CollisionReportPtr;


class CollisionQuery: public TinySG::Query
{
public:
	CollisionQuery();
	virtual ~CollisionQuery();

	// Inherited from Query
	virtual TinySG::QueryResult* execute(const TinySG::ObjectManager* mgr, const TinySG::PropertyCollection* params = 0);

	static void collisionCallback(void*, dGeomID, dGeomID);
};

}

#endif /* COLLISIONQUERY_H_ */
