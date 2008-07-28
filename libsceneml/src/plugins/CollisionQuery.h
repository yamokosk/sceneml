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

using namespace sml;

class CollisionQuery: public sml::SceneQuery
{
public:
	CollisionQuery(SceneMgr* mgr);
	virtual ~CollisionQuery();

	// Inherited from SceneQuery
	virtual SceneQueryResult* query();
};

}

#endif /* COLLISIONQUERY_H_ */
