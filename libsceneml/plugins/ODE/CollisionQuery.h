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

class CollisionQuery: public tinysg::SceneQuery
{
public:
	CollisionQuery();
	virtual ~CollisionQuery();

	// Inherited from SceneQuery
	virtual tinysg::SceneQuery* clone() const;
	virtual void execute(const tinysg::SceneManager* mgr);
	virtual std::string getType() const;

	virtual tinysg::QueryResult* getResult();
	virtual void deleteResult( tinysg::QueryResult* result );

private:
	bool inCollision_;
};

class SimpleResult : public tinysg::QueryResult
{
public:
	SimpleResult(tinysg::SceneQuery* creator);
	virtual ~SimpleResult();

	bool inCollision();
	void _setCollisionStatus(bool status);
	virtual std::string getType() const;

protected:
	bool inCollision_;
};

void collisionCallback(void* data, dGeomID o1, dGeomID o2);

}

#endif /* COLLISIONQUERY_H_ */
