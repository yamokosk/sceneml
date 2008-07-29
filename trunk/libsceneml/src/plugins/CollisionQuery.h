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

namespace smlode {

struct CollisionResult : public sml::QueryObjectList
{

};

class CollisionQuery: public sml::SceneObjectQuery
{
public:
	CollisionQuery(sml::SceneMgr* mgr);
	virtual ~CollisionQuery();

	// Inherited from SceneQuery
	virtual sml::QueryResult* query();
};

}

#endif /* COLLISIONQUERY_H_ */
