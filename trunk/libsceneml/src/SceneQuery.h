/*
 * SceneQuery.h
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#ifndef SCENEQUERY_H_
#define SCENEQUERY_H_

namespace sml {

typedef std::list<SceneObject*> SceneQueryResultObjectList;

struct SceneQueryResult
{
	/// List of movable objects in the query (entities, particle systems etc)
	SceneQueryResultObjectList movables;
};

class SceneQuery {
public:
	SceneQuery(SceneMgr* mgr);
	virtual ~SceneQuery();

	virtual SceneQueryResult* query() = 0;

protected:
	SceneMgr* manager_;
};

}

#endif /* SCENEQUERY_H_ */
