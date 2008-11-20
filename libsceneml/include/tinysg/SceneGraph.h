/*************************************************************************
 * TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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

#ifndef _TINYSG_SCENEMGR_H_FILE_
#define _TINYSG_SCENEMGR_H_FILE_

// External includes
#include <string>
#include <utility>
#include <queue>

// Internal includes
#include <tinysg/Map.h>
#include <tinysg/Archive.h>
#include <tinysg/ObjectManager.h>
#include <tinysg/SceneNode.h>

namespace TinySG {

class SceneGraph : public ObjectManager
{
	static const std::string NodeType;
	static unsigned long NextGeneratedNameExt;

public:
	// Name of world node
	static const std::string World;

	SceneGraph();
	virtual ~SceneGraph();

	// Node management
	SceneNode* createNode(const PropertyCollection* params=NULL);
	SceneNode* createNode(const std::string& name, const PropertyCollection* params=NULL);
	void destroyNode(const std::string& name);
	void destroyNode(SceneNode* obj);
	void destroyAllNodes();
	SceneNode* getNode(const std::string& name) const;
	const SceneNode* getRootNode() const {return rootNode_;}

	// Scene management
	void clearScene();
	void update();

	// Save state
	void save(TinySG::Archive& ar);
	void load(TinySG::Archive& ar);

private:
	SceneNode* rootNode_;
	SceneNodeFactory nodeFactory_;
};

}

#endif
