/*
 * CollisionQuery.h
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#ifndef COLLISIONQUERY_H_
#define COLLISIONQUERY_H_

// SceneML
#include <SceneML.h>

// ODE library
#include <ode/ode.h>

#define NUM_CONTACT_POINTS 1

namespace smlode {

class CollisionQuery: public sml::SceneQuery
{
public:
	CollisionQuery();
	virtual ~CollisionQuery();

	// Inherited from SceneQuery
	virtual SceneQuery* clone() const;
	virtual void execute(const SceneManager* mgr);
	virtual std::string getType() const;

	virtual QueryResult* getResult();
	virtual void deleteResult( QueryResult* result );

private:
	bool inCollision_;
};

class SimpleResult : public sml::QueryResult
{
public:
	SimpleResult(sml::SceneQuery* creator);
	virtual ~SimpleResult();

	bool inCollision();
	void _setCollisionStatus(bool status);

protected:
	bool inCollision_;
};

void collisionCallback(void* data, dGeomID o1, dGeomID o2);

}

#endif /* COLLISIONQUERY_H_ */
