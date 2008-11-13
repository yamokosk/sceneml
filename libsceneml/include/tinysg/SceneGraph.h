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

namespace TinySG {

// Forward declarations
class SceneNode;
class Query;
class QueryResult;

class SceneGraph : public ObjectManager
{
public:
	typedef MAP<std::string, SceneNode*> Nodes;
	typedef MAP<std::string, Query*> Queries;

	SceneGraph();
	virtual ~SceneGraph();

	// Node management
	virtual SceneNode* createNode(const PropertyCollection* params=NULL);
	virtual SceneNode* createNode(const std::string& name, const PropertyCollection* params=NULL);
	virtual void destroyNode(const std::string& name);
	virtual void destroyNode(SceneNode* obj);
	virtual void destroyAllNodes();
	virtual SceneNode* getNode(const std::string& name) const;

	// Scene management
	void clearScene();
	void update();

	// Query management
	virtual QueryResult* performQuery( const std::string queryName, const PropertyCollection* params=0 );
	virtual void addQuery( Query* query );

	// Save state
	void save(TinySG::Archive& ar);
	void load(TinySG::Archive& ar);

private:
	SceneNode* rootNode_;
	Nodes nodes_;
	Queries queries_;
};

}

#endif
