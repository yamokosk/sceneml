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
#include "Exception.h"
#include "Map.h"
#include "Object.h"
#include "math/Math.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"

namespace TinySG {

/*
// interface to all car 'parts'
struct NodeVisitor
{
	virtual void visit(Node& node) = 0;
};

class UpdateVisitor : public NodeVisitor
{
	virtual void visit(Node& node)
	{
		if ( node->hasParent() )
		{
			if ( node->inheritOrientation() ) {
				// Combine orientation with that of parent
				node->derivedOrientation_ = node->getParentOrientation() * orientation_;
			} else {
				// No inheritance
				derivedOrientation_ = orientation_;
			}

			// Update scale
			if ( node->inheritScale() ) {
				// Scale own position by parent scale, NB just combine
				// as equivalent axes, no shearing
				//derivedScale_ = parentScale * scale_;
				node->derivedScale_ = node->getParentScale();
			} else {
				// No inheritance
				derivedScale_ = scale_;
			}

			// Change position vector based on parent's orientation & scale
			//derivedPosition_ = parentOrientation * (parentScale_ * position_);
			derivedPosition_ = node->getParentOrientation() * position_;

			// Add altered position vector to parents
			derivedPosition_ += node->getParentPosition();
		} else {
			// Root node, no parent
			node->derivedOrientation_ = orientation_;
			node->derivedPosition_ = position_;
			node->derivedScale_ = scale_;
		}

		node->cachedTransformOutOfDate_ = true;
	}
};*/

/*!
@ingroup TinySG
@brief Models a node in a tree-like data structure

The Node class represents the nodes associated with
 */

class LevelComparison;

class Node : public Object
{
	//friend class NodeVisitor;

	// For logging
	static log4cxx::LoggerPtr logger;

	// Friends to the Node class
	friend ostream& operator << (ostream& os, const TinySG::Node& s);

protected:
	typedef std::set< Node * >		ChildUpdateSet;
	typedef std::vector< Node* > QueuedUpdates;
	typedef std::priority_queue< Node*, vector<Node*>, LevelComparison  >	NodesToUpdate;
	typedef MAP<std::string, Node*>		ChildNodeMap;
	typedef ChildNodeMap::iterator		ChildNodeIterator;
	typedef ChildNodeMap::const_iterator	ConstChildNodeIterator;

public:

	// Static bit masks
	static const uint8_t NeedParentUpdateBit = 0x01;
	static const uint8_t UnusedBit1 = 0x02;
	static const uint8_t CachedTransformOutOfDateBit = 0x04;
	static const uint8_t UnusedBit2 = 0x08;
	static const uint8_t UnusedBit3 = 0x10;
	static const uint8_t QueuedForUpdateBit = 0x20;
	static const uint8_t OrientationInheritedBit = 0x40;
	static const uint8_t ScaleInheritedBit = 0x80;

	static const std::string ObjectTypeID;

	Node();
	virtual ~Node();

	// From Object
	virtual Object* clone() const;

	unsigned int getLevel() const {return level_;}

	// Operator
	bool operator< (const Node&);
	// Flag management
	bool queryFlag( uint8_t bit ) const {return (flags_ & bit);}
	uint8_t getFlags() const {return flags_;}
	void setFlags( uint8_t bits ) {flags_ = bits;}

	// Child management
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
	//! Tells us if this node has a parent
	bool hasParent(void) {return (parent_ != NULL);}

	// Spatial management
	//! Sets the orientation of this node via a Quaternion.
	void setOrientation (const Quaternion &q);
	//! Sets the orientation of this node via Quaternion parameters.
	void setOrientation (Real w, Real x, Real y, Real z);
	//! Sets the position of the node relative to it's parent.
	void setPosition(const ColumnVector &pos);
	//! Sets the position of the node relative to it's parent.
	void setPosition(Real x, Real y, Real z);
	//! Sets the scale of the node relative to it's parent.
	void setScale(const ColumnVector &s);
	//! Sets the scale of the node relative to it's parent.
	void setScale(Real x, Real y, Real z);
	//! Returns a Quaternion representing the transform's orientation.
	const Quaternion& getOrientation(void) const {return orientation_;}
	//! Gets the position of the node relative to it's parent.
	const ColumnVector& getPosition(void) const {return position_;}
	//! Returns the scale associated with this transform
	const ColumnVector& getScale(void) const {return scale_;}
	//! Gets a matrix whose columns are the local axes based on the nodes orientation relative to it's parent.
	//ReturnMatrix getLocalAxes (void) const;
	//! Returns full 4x4 matrix representation of the transform
	const Matrix& getFullTransform (void);
	//! Returns the parent's derived orientation
	const Quaternion& getParentOrientation() const;
	const ColumnVector& getParentPosition() const;
	const ColumnVector& getParentScale() const;
	//! Returns this node's derived orientation/position/scale
	const Quaternion& getDerivedOrientation();
	const ColumnVector& getDerivedPosition();
	const ColumnVector& getDerivedScale();

	// Visitor stuff
	//virtual void accept(NodeVisitor& visitor);

	// Update management
	static void dequeueForUpdate(Node *n);
	static void queueForUpdate(Node* n);
	static void processQueuedUpdates (void);
	static void addToUpdateQueue(Node* n);
	static void updateNodes(void);

	void updateFromParent();
	void needUpdate();

	/*
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
*/
protected:
/*
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
*/
	//! See Node.
	void setParent(Node *parent);
	//! Internal method called to update the cached transform
	virtual void updateCachedTransform();

	//! Pointer to parent node.
	Node* parent_;
	//! level in the tree
	unsigned int level_;
	//! Stores the orientation of the node relative to it's parent.
	Quaternion orientation_;
	//! Stores the position/translation of the node relative to its parent.
	ColumnVector position_;
	//! Stores the scaling factor applied to this node.
	ColumnVector scale_;
	//! Stores the orientation of the node relative to it's parent.
	Quaternion derivedOrientation_;
	//! Stores the position/translation of the node relative to its parent.
	ColumnVector derivedPosition_;
	//! Stores the scaling factor applied to this node.
	ColumnVector derivedScale_;
	//! Cached transform
	Matrix cachedTransform_;
	//! Collection of pointers to direct children; hashmap for efficiency.
	ChildNodeMap children_;
	//! List of children which need updating, used if self is not out of date but children are.
	//ChildUpdateSet childrenToUpdate_;
	//! Queue of nodes which still need updating
	static QueuedUpdates queuedUpdates_;
	//! Queue of nodes to update
	static NodesToUpdate nodesToUpdate_;
	//! Various flag states stored in 8-bit unsigned integer.
	uint8_t flags_;
};

class LevelComparison
{
public:
	bool operator()(const Node *a, const Node *b) const
	// Note that if a < b and b < a are both false, then a = b wr.t. to this comparison
	// Be sure to implement this function as "strictly less than"
	{
		return (a->getLevel() > b->getLevel());
	}
};


class NodeFactory : public ObjectFactory
{
protected:
	// To be overloaded by specific object factories
	virtual Object* createInstanceImpl(const PropertyCollection* params = 0);

public:
	NodeFactory() : ObjectFactory(Node::ObjectTypeID) {};
	virtual ~NodeFactory() {};

	// To be overloaded by specific object factories
	virtual void destroyInstance(Object* obj);
};


class NodeVisitor
{

};

} // namespace TinySG

ostream& operator << (ostream& os, const TinySG::Node& s);

 #endif
