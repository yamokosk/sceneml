/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version. The text of the GNU Lesser General
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for
 * more details.
 *
 *************************************************************************/

#ifndef _SML_SCENEMGR_H_FILE_
#define _SML_SCENEMGR_H_FILE_

// std includes
#include <string>
#include <map>

// sml includes
#include "Exception.h"
#include "CommandStack.h"

namespace sml {

// Forward declarations
class Node;

class SceneMgr
{
// public types
public:
	typedef std::map<std::string, Node*>	NodeMap;
	typedef NodeMap::iterator				NodeIterator;
	typedef NodeMap::const_iterator			ConstNodeIterator;

public:
	SceneMgr();
	virtual ~SceneMgr();

	Node* createNode();
	Node* createNode(const std::string& name);
	void destroySceneNode(const std::string& name);
	Node* getRootNode() const;
	Node* getNode(const std::string& name) const;
	virtual bool hasSceneNode(const std::string& name) const;

	void clearScene();

	void _updateSceneGraph();

	typedef std::map<std::string, SceneObject*> SceneObjectMap;
	struct SceneObjectCollection
	{
		SceneObjectMap map;
		OGRE_MUTEX(mutex)
	};
	typedef std::map<std::string, SceneObjectCollection*> SceneObjectCollectionMap;
	SceneObjectCollectionMap sceneObjectCollectionMap_;
	SceneObjectCollection* getSceneObjectCollection(const std::string& typeName);
	const SceneObjectCollection* getSceneObjectCollection(const std::string& typeName) const;

	virtual SceneObject* createSceneObject(const std::string& name,const std::string& typeName, const NameValuePairList* params = 0);
	virtual void destroySceneObject(const std::string& name, const std::string& typeName);
	virtual void destroySceneObject(SceneObject* m);
	virtual void destroyAllSceneObjectsByType(const std::string& typeName);
	virtual void destroyAllSceneObjects(void);
	virtual SceneObject* getSceneObject(const std::string& name, const std::string& typeName) const;
	virtual bool hasSceneObject(const std::string& name, const std::string& typeName) const;
	typedef MapIterator<SceneObjectMap> SceneObjectIterator;
	virtual SceneObjectIterator getSceneObjectIterator(const std::string& typeName);
	virtual void injectSceneObject(SceneObject* m);
	virtual void extractSceneObject(const std::string& name, const std::string& typeName);
	virtual void extractSceneObject(SceneObject* m);
	virtual void extractAllSceneObjectsByType(const std::string& typeName);

private:
	NodeMap nodes_;
	Node* rootNode_;

	//SpaceMap spaces_;

	CommandStack cmdStack_;
};

}

#endif
