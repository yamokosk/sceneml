/*
 * CollisionQuery.h
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#ifndef COLLISIONQUERY_H_
#define COLLISIONQUERY_H_

#include "SceneQuery.h"

namespace sml {

namespace ode {

// Forward declaration
class SceneMgr;

class CollisionQuery: public sml::SceneQuery
{
public:
	CollisionQuery(SceneMgr* mgr);
	virtual ~CollisionQuery();

	// Inherited from SceneQuery
	virtual void query();
};

}

}

#endif /* COLLISIONQUERY_H_ */
