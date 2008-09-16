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

#ifndef _TINYSG_SCENEMGR_H_FILE_
#define _TINYSG_SCENEMGR_H_FILE_

// std library includes
#include <string>
#include <utility>
#include <queue>

// Boost includes
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

// Internal includes
#include "Root.h"
#include "HashMap.h"
#include "Exception.h"
#include "Entity.h"
#include "PropertyCollection.h"


namespace TinySG {

// Forward declarations
class Node;

class SceneManager : public Observer
{
	// For serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version);

// public types
public:
	// Nodes
	typedef stdext::hash_map<std::string, Node*>	NodeMap;
	typedef NodeMap::iterator				NodeIterator;
	typedef NodeMap::const_iterator			NodeConstIterator;

	// Queries
	typedef std::queue< SceneQuery* > QueryQueue;

	// Query results
	typedef stdext::hash_map<std::string, QueryResult*> ResultMap;
	typedef ResultMap::iterator ResultMapIterator;

public:
	SceneManager();
	virtual ~SceneManager();
	void clearScene();

	// Nodes
	Node* createNode();
	Node* createNode(const std::string& name);
	void destroyNode(const std::string& name);
	Node* getRootNode() const;
	Node* getNode(const std::string& name) const;
	virtual bool hasSceneNode(const std::string& name) const;

	// Queries
	QueryResult* getQueryResult(const std::string& typeName);
	void destroyAllQueryResults();
	void _addResult(QueryResult* result);

	// Update scene
	void update();

	// Internal functions
	void _addQuery(SceneQuery* query);
	void _performQueries();

	void _updateSceneGraph();

	// From Observer class
	void update(int hint);

private:
	Node* rootNode_;
	NodeMap nodes_;

	QueryQueue queries_;
	ResultMap results_;
};

BOOST_CLASS_VERSION(SceneManager, TINYSG_VERSION)

}

#endif
