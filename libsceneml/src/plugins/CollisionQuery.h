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

class CollisionQuery: public sml::SceneObjectQuery
{
public:
	CollisionQuery();
	virtual ~CollisionQuery();

	// Inherited from SceneQuery
	virtual SceneObjectQuery* clone() const;
	virtual QueryResult execute(const SceneMgr* mgr);
	virtual std::string getType() const;
};

}

#endif /* COLLISIONQUERY_H_ */
