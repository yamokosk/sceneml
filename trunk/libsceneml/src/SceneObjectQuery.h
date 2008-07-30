/*
 * SceneQuery.h
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#ifndef SCENEQUERY_H_
#define SCENEQUERY_H_

#include <list>

#include <SceneObject.h>

namespace sml {

typedef std::list<SceneObject*> SceneObjectList;

struct QueryResult
{
	SceneObjectList objects_;
};

class SceneObjectQuery
{
public:
	SceneObjectQuery();
	virtual ~SceneObjectQuery();

	virtual SceneObjectQuery* clone() const = 0;
	virtual QueryResult execute(const SceneMgr* mgr) = 0;
	virtual std::string getType() const = 0;
};

}

#endif /* SCENEQUERY_H_ */
