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

#ifndef NODE_H
#define NODE_H

// External includes
#include <string>
#include <set>
#include <queue>
#include <iostream>
#include <log4cxx/logger.h>

// Local includes
#include "Map.h"
#include "Object.h"

namespace TinySG {

/*!
@ingroup TinySG
@brief Models a node in a tree-like data structure

The Node class represents the nodes associated with
 */
class Node : public Object
{
	// For logging
	static log4cxx::LoggerPtr logger;

	// Friends to the Node class
	friend ostream& operator << (ostream& os, const TinySG::Node& s);

protected:
	typedef std::set< Node * >		ChildUpdateSet;
	typedef std::queue< Node * >	QueuedUpdates;
	typedef MAP<std::string, Node*>		ChildNodeMap;
	typedef ChildNodeMap::iterator		ChildNodeIterator;
	typedef ChildNodeMap::const_iterator	ConstChildNodeIterator;

	// Static bit masks
	static const uint8_t NeedParentUpdateMask = 0x01;
	static const uint8_t NeedChildUpdateMask = 0x02;
	static const uint8_t UpdateChildrenMask = 0x04;
	static const uint8_t ParentHasChangedMask = 0x08;
	static const uint8_t ParentNotifiedMask = 0x10;
	static const uint8_t QueuedForUpdateMask = 0x20;
	static const uint8_t OrientationInheritedMask = 0x40;
	static const uint8_t ScaleInheritedMask = 0x80;

public:
	static const std::string ObjectTypeID;

	Node();
	virtual ~Node();

	//! Adds a (precreated) child scene node to this node.
	void addChild (Node* child);
	//! Reports the number of child nodes under this one.
	unsigned short numChildren (void) const;
	//! Gets a pointer to a child node.
	Node* getChild (unsigned short index) const;
	//! Gets a pointer to a named child node.
	Node* getChild (const std::string& name) const;
	//! Drops the specified child from this node.
	Node* removeChild (unsigned short index);
	//! Drops the specified child from this node.
	Node* removeChild (Node *child);
	//! Drops the named child from this node.
	Node* removeChild (const std::string &name);
	//! Removes all child Nodes attached to this node.
	void removeAllChildren (void);
	//! Gets the parent of this SceneNode.
	Node* getParent(void) const;

	//! To be called in the event of transform changes to this node that require it's recalculation.
	void needUpdate (bool forceParentUpdate=false);
	//! Called by children to notify their parent that they need an update.
	void requestUpdate (Node *child, bool forceParentUpdate=false);
	//! Called by children to notify their parent that they no longer need an update.
	void cancelUpdate (Node *child);
	//! Internal method to update the Node.
	void _update (bool updateChildren, bool parentHasChanged);

	static void dequeueForUpdate(Node *n);
	static void queueForUpdate(Node* n);
	bool queuedForUpdate();

protected:
	//! Queue a 'needUpdate' call to a node safely.
	static void queueNeedUpdate(Node *n);
	//! Process queued 'needUpdate' calls.
	static void processQueuedUpdates (void);
	// Remarks: Splitting the implementation of the update away from the update call
	// itself allows the detail to be overridden without disrupting the general
	// sequence of updateFromParent (e.g. raising events)
	void updateFromParentImpl(void);
	//! Triggers the node to update it's combined transforms.
	void _updateFromParent (void);

	//! See Node.
	virtual Node* createChildImpl(void);
	virtual Node* createChildImpl(const std::string& name);

	//! See Node.
	void setParent(Node *parent);

	//! Pointer to parent node.
	Node* parent_;
	//! Collection of pointers to direct children; hashmap for efficiency.
	ChildNodeMap children_;
	//! List of children which need updating, used if self is not out of date but children are.
	ChildUpdateSet childrenToUpdate_;
	//! Queue of nodes which still need updating
	static QueuedUpdates queuedUpdates_;
	//! Various flag states stored in 8-bit unsigned integer.
	uint8_t flags_;
};


class NodeFactory
{
protected:
	// To be overloaded by specific object factories
	virtual Object* createInstanceImpl(const std::string& name, const PropertyCollection* params = 0);

public:
	NodeFactory() : ObjectFactory(Node::ObjectTypeID) {};
	virtual ~NodeFactory() {};

	// To be overloaded by specific object factories
	virtual void destroyInstance(Object* obj) = 0;
};


} // namespace TinySG

ostream& operator << (ostream& os, const TinySG::Node& s);

 #endif
