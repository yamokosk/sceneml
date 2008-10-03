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
/*
 * MovableNode.h
 *
 *  Created on: Sep 19, 2008
 *      Author: yamokosk
 */

#ifndef MOVABLENODE_H_
#define MOVABLENODE_H_


namespace TinySG {

class MovableNode : public CoordinateTransform, public Node
{
	// Used for unnamed nodes
	static unsigned int UnnamedIndex;

public:
	typedef MAP<std::string, Entity*>	ObjectMap;
	typedef ObjectMap::iterator 		ObjectMapIterator;
	typedef ObjectMap::const_iterator	ObjectMapConstIterator;

	enum TransformSpace
	{
		/// Transform is relative to the local space
		TS_LOCAL,
		/// Transform is relative to the space of the parent node
		TS_PARENT,
		/// Transform is relative to world space
		TS_WORLD
	};

	//! Creates an unnamed new Node as a child of this node.
	MovableNode* createChild(const ColumnVector& translate=VectorFactory::Vector3( ZERO ),
					  const Quaternion& rotate=QuaternionFactory::IDENTITY );
	//! Creates a new named Node as a child of this node.
	MovableNode* createChild(const std::string& name,
					  const ColumnVector& translate=VectorFactory::Vector3( ZERO ),
					  const Quaternion& rotate=QuaternionFactory::IDENTITY );

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

	//! Gets the orientation of the node as derived from all parents.
	Quaternion _getDerivedOrientation (void);
	//! Gets the position of the node as derived from all parents.
	const ColumnVector& _getDerivedPosition (void);
	//! Gets the scaling factor of the node as derived from all parents.
	const ColumnVector& _getDerivedScale (void);

	//! Scene objects attached to this node
	ObjectMap sceneObjects_;
};

#endif /* MOVABLENODE_H_ */
