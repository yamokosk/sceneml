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
	typedef MAP<std::string, Node*>		ChildNodeMap;
	typedef ChildNodeMap::iterator		ChildNodeIterator;
	typedef ChildNodeMap::const_iterator	ConstChildNodeIterator;

public:
	static const uint8_t ParentChangedBit = 0x01;
	static const uint8_t PoseChangedBit = 0x02;
	static const uint8_t ScaleChangedBit = 0x04;
	static const std::string ObjectTypeID;

	Node();
	virtual ~Node();
	// From Object
	virtual Object* clone() const;

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

	// Update management
	virtual void update(uint8_t flags=0);
	virtual void notifyUpdate(uint8_t hint=0);

protected:
	//! See Node.
	void setParent(Node *parent);
	//! Internal method called to update the cached transform
	void updateCachedTransform();

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
	//! Whether or not this node has a valid world transform.
	bool validWorldTransform_;
	bool cachedTransformOutOfDate_;
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

} // namespace TinySG

ostream& operator << (ostream& os, const TinySG::Node& s);

#endif
