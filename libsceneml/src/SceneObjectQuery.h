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

class SceneQuery
{
public:
	SceneQuery();
	virtual ~SceneQuery();

	virtual SceneQuery* clone() const = 0;
	virtual void execute(const SceneManager* mgr) = 0;
	virtual std::string getType() const = 0;
};

}

#endif /* SCENEQUERY_H_ */
