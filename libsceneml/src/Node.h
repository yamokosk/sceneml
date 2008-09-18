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

// std inlcudes
#include <string>
#include <set>
#include <iostream>

// Internal includes
#include "Object.h"
#include "SceneManager.h"
#include "Entity.h"
#include "HashMap.h"
#include "math/Math.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"


namespace TinySG {

class Node : public Object
{
	// Friends to the Node class
	friend class SceneManager;
	friend ostream& operator << (ostream& os, const TinySG::Node& s);

// Protected static data
protected:
	static QueuedUpdates queuedUpdates_;

	// Static bit masks
	static const uint8_t NeedParentUpdateMask = 0x01;
	static const uint8_t NeedChildUpdateMask = 0x02;
	static const uint8_t UpdateChildrenMask = 0x04;
	static const uint8_t ParentHasChangedMask = 0x08;
	static const uint8_t ParentNotifiedMask = 0x10;
	static const uint8_t QueuedForUpdateMask = 0x20;
	static const uint8_t OrientationInheritedMask = 0x40;
	static const uint8_t ScaleInheritedMask = 0x80;

// Public types and enums
public:
	typedef HashMap<std::string, Node*>		ChildNodeMap;
	typedef ChildNodeMap::iterator			ChildNodeIterator;
	typedef ChildNodeMap::const_iterator	ConstChildNodeIterator;

	typedef HashMap<std::string, Entity*> ObjectMap;
	typedef ObjectMap::iterator ObjectMapIterator;
	typedef ObjectMap::const_iterator ObjectMapConstIterator;

	enum TransformSpace
	{
		/// Transform is relative to the local space
		TS_LOCAL,
		/// Transform is relative to the space of the parent node
		TS_PARENT,
		/// Transform is relative to world space
		TS_WORLD
	};

public:
	Node();
	virtual ~Node();

	//! Adds an instance of a scene object to this node.
	void attachObject (Entity *obj);
	//! Reports the number of objects attached to this node.
	unsigned short numAttachedObjects (void) const;
	//! Retrieves a pointer to an attached object.
	Entity* getAttachedObject (unsigned short index);
	//! Retrieves a pointer to an attached object.
	Entity* getAttachedObject (const std::string &name);
	//! Detaches the indexed object from this scene node.
	Entity* detachObject (unsigned short index);
	//! Detaches an object by pointer.
	void detachObject (Entity *obj);
	//! Detaches the named object from this node and returns a pointer to it.
	Entity* detachObject (const std::string &name);
	//! Detaches all objects attached to this node.
	void detachAllObjects (void);

	//! Returns a Quaternion representing the nodes orientation.
	const Quaternion& getOrientation () const;
	//! Sets the orientation of this node via a Quaternion.
	void setOrientation (const Quaternion &q);
	//! Sets the orientation of this node via Quaternion parameters.
	void setOrientation (Real w, Real x, Real y, Real z);
	//! Resets the nodes orientation (local axes as world axes, no rotation).
	void resetOrientation (void);
	//! Sets the position of the node relative to it's parent.
	void setPosition(const ColumnVector &pos);
	//! Sets the position of the node relative to it's parent.
	void setPosition(Real x, Real y, Real z);
	//! Gets the position of the node relative to it's parent.
	const ColumnVector& getPosition(void) const;
	//! Scales the node, combining it's current scale with the passed in scaling factor.
	void scale(const ColumnVector &scale);
	//! Scales the node, combining it's current scale with the passed in scaling factor.
	void scale(Real x, Real y, Real z);
	//! Moves the node along the cartesian axes.
	void translate(const ColumnVector &d, TransformSpace relativeTo=TS_PARENT);
	//! Moves the node along the cartesian axes.
	void translate(Real x, Real y, Real z, TransformSpace relativeTo=TS_PARENT);
	//! Moves the node along arbitrary axes.
	void translate(const SquareMatrix& axes, const ColumnVector &move, TransformSpace relativeTo=TS_PARENT);
	//! Moves the node along arbitrary axes.
	void translate(const SquareMatrix& axes, Real x, Real y, Real z, TransformSpace relativeTo=TS_PARENT);
	//! Rotate the node around an arbitrary axis.
	void rotate(const ColumnVector &axis, Real angle, TransformSpace relativeTo=TS_LOCAL);
	//! Rotate the node around an aritrary axis using a Quarternion.
	void rotate(const Quaternion &q, TransformSpace relativeTo=TS_LOCAL);
	//! Gets a matrix whose columns are the local axes based on the nodes orientation relative to it's parent.
	ReturnMatrix getLocalAxes (void) const;
	//! Gets the orientation of the node as derived from all parents.
	Quaternion _getDerivedOrientation (void);
	//! Gets the position of the node as derived from all parents.
	const ColumnVector& _getDerivedPosition (void);
	//! Gets the scaling factor of the node as derived from all parents.
	const ColumnVector& _getDerivedScale (void);
	//! Gets the full transformation matrix for this node.
	const Matrix& _getFullTransform (void);
	//! Public form of the above function
	const Matrix& getFullTransform (void) const;

	//! Creates an unnamed new Node as a child of this node.
	Node* createChild(const ColumnVector& translate=VectorFactory::Vector3( ZERO ),
					  const Quaternion& rotate=QuaternionFactory::IDENTITY );
	//! Creates a new named Node as a child of this node.
	Node* createChild(const std::string& name,
					  const ColumnVector& translate=VectorFactory::Vector3( ZERO ),
					  const Quaternion& rotate=QuaternionFactory::IDENTITY );
	//! Adds a (precreated) child scene node to this node.
	void addChild (Node* child);
	//! Reports the number of child nodes under this one.
	unsigned short numChildren (void) const;
	//! Gets a pointer to a child node.
	Node* getChild (unsigned short index) const
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

	//! Sets the current transform of this node to be the 'initial state' ie that position / orientation / scale to be used as a basis for delta values used in keyframe animation.
	void setInitialState (void);
	//! Resets the position / orientation / scale of this node to it's initial state, see setInitialState for more info.
	void resetToInitialState (void);
	//! Gets the initial position of this node, see setInitialState for more info.
	const ColumnVector& getInitialPosition (void) const;
	//! Gets the initial orientation of this node, see setInitialState for more info.
	const Quaternion& getInitialOrientation (void) const;
	//! Gets the initial position of this node, see setInitialState for more info.
	const ColumnVector& getInitialScale (void) const;

	//! To be called in the event of transform changes to this node that require it's recalculation.
	void needUpdate (bool forceParentUpdate=false);
	//! Called by children to notify their parent that they need an update.
	void requestUpdate (Node *child, bool forceParentUpdate=false);
	//! Called by children to notify their parent that they no longer need an update.
	void cancelUpdate (Node *child);
	//! Internal method to update the Node.
	void _update (bool updateChildren, bool parentHasChanged);

// Static Public Member Functions
protected:
	//! Queue a 'needUpdate' call to a node safely.
	static void queueNeedUpdate(Node *n);
	//! Process queued 'needUpdate' calls.
	static void processQueuedUpdates (void);

// Protected Types
protected:
	typedef std::set< Node * >		ChildUpdateSet;
	typedef std::vector< Node * >	QueuedUpdates;

//Protected Member Functions
protected:
	// Remarks: Splitting the implementation of the update away from the update call
	// itself allows the detail to be overridden without disrupting the general
	// sequence of updateFromParent (e.g. raising events)
	void updateFromParentImpl(void);
	//! Triggers the node to update it's combined transforms.
	void _updateFromParent (void);

	//! See Node.
	Node* 	createChildImpl(void);
	//! See Node.
	Node* 	createChildImpl(const std::string& name);

	//! See Node.
	void setParent(Node *parent);
	/*
	//! Internal method for setting whether the node is in the scene graph.
	virtual void setInSceneGraph (bool inGraph);*/

// Protected Attributes
protected:
	//! Scene objects attached to this node
	ObjectMap sceneObjects_;
	//! Pointer to parent node.
	Node* parent_;
	//! Collection of pointers to direct children; hashmap for efficiency.
	ChildNodeMap children_;
	//! List of children which need updating, used if self is not out of date but children are.
	ChildUpdateSet childrenToUpdate_;

	//! Stores the orientation of the node relative to it's parent.
	Quaternion orientation_;
	//! Stores the position/translation of the node relative to its parent.
	ColumnVector position_;
	//! Stores the scaling factor applied to this node.
	ColumnVector scale_;

	//! Cached combined orientation.
	Quaternion derivedOrientation_;
	//! Cached combined position.
	ColumnVector derivedPosition_;
	//! Cached combined scale.
	ColumnVector derivedScale_;

	//! The position to use as a base for keyframe animation.
	ColumnVector initialPosition_;
	//! The orientation to use as a base for keyframe animation.
	Quaternion initialOrientation_;
	//! The scale to use as a base for keyframe animation.
	ColumnVector initialScale_;

	//! Various flag states stored in 8-bit unsigned integer.
	uint8_t flags_;

	//! Cached derived transform as a 4x4 matrix.
	mutable Matrix cachedTransform_;

	//! Cached derived transform as a 4x4 matrix.
	mutable bool cachedTransformOutOfDate_;
};


class NodeFactory
{
protected:
	// To be overloaded by specific object factories
	virtual Object* createInstanceImpl(const std::string& name, const PropertyCollection* params = 0);

public:
	NodeFactory() : ObjectFactory("TinySG_Node") {};
	virtual ~NodeFactory() {};

	// To be overloaded by specific object factories
	virtual void destroyInstance(Object* obj) = 0;
};


} // namespace TinySG

ostream& operator << (ostream& os, const TinySG::Node& s);

 #endif
