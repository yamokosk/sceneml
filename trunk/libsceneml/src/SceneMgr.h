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
#include <smlException.h>

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

	typedef std::map<std::string, Space*>	SpaceMap;
	typedef SpaceMap::iterator				SpaceIterator;
	typedef SpaceMap::const_iterator			ConstSpaceIterator;

public:
	SceneMgr();
	virtual ~SceneMgr();

	Node* getRootNode() const {return rootNode_;}
	Node* createNode();
	Node* createNode(const std::string& name);

	Space* createSpace(const PropertyCollection& pc);
	void addCollisionPair(const std::string& space1, const std::string& space2 );

	Body* createBody( const PropertyCollection& pc );
private:
	NodeMap nodes_;
	Node* rootNode_;

	SpaceMap spaces_;

	CommandStack cmdStack_;
};

};

#endif
