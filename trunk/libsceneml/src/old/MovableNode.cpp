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
 * MovableNode.cpp
 *
 *  Created on: Sep 19, 2008
 *      Author: yamokosk
 */


namespace TinySG
{

unsigned int MovableNode::UnnamedIndex(0);

//! Creates an unnamed new MovableNode as a child of this node.
MovableNode* MovableNode::createChild(const ColumnVector& translate, const Quaternion& rotate)
{
	std::stringstream ss; ss << "Node" << MovableNode::UnnamedIndex++ << std::endl;
	return createChild(ss.str(), translate, rotate);
}

//! Creates a new named MovableNode as a child of this node.
MovableNode* MovableNode::createChild(const std::string& name, const ColumnVector& translate, const Quaternion& rotate)
{
	MovableNode* newNode = createChildImpl(name);
	newNode->translate(translate);
	newNode->rotate(rotate);
	this->addChild(newNode);

	return newNode;
}

//! Moves the node along the cartesian axes.
void MovableNode::translate(const ColumnVector &d, TransformSpace relativeTo)
{
	switch(relativeTo)
	{
	case TS_LOCAL:
		// position is relative to parent so transform downwards
		position_ += (orientation_ * d);
		break;
	case TS_WORLD:
		// position is relative to parent so transform upwards
		if (parent_)
		{
			Quaternion qi = inverse( parent_->_getDerivedOrientation() );
			position_ += (qi * d); //	/ parent_->_getDerivedScale();
		}
		else
		{
			position_ += d;
		}
		break;
	case TS_PARENT:
		position_ += d;
		break;
	}
	needUpdate();
}

//! Moves the node along the cartesian axes.
void MovableNode::translate(Real x, Real y, Real z, TransformSpace relativeTo)
{
	ColumnVector v; v << x << y << z;
	translate(v, relativeTo);
}

//! Moves the node along arbitrary axes.
void MovableNode::translate(const SquareMatrix &axes, const ColumnVector &move, TransformSpace relativeTo)
{
	ColumnVector derived = axes * move;
	translate(derived, relativeTo);
}

//! Moves the node along arbitrary axes.
void MovableNode::translate(const SquareMatrix &axes, Real x, Real y, Real z, TransformSpace relativeTo)
{
	ColumnVector d(3); d << x << y << z;
	translate(axes,d,relativeTo);
}

//! Rotate the node around an arbitrary axis.
void MovableNode::rotate(const ColumnVector &axis, Real angle, TransformSpace relativeTo)
{
	Quaternion q = QuatFromAngleAxis(angle,axis);
	rotate(q, relativeTo);
}

//! Rotate the node around an aritrary axis using a Quarternion.
void MovableNode::rotate(const Quaternion &q, TransformSpace relativeTo)
{
	switch(relativeTo)
	{
	case TS_LOCAL:
		// Note the order of the mult, i.e. q comes after
		orientation_ = orientation_ * q;
		break;
	case TS_WORLD:
		// Rotations are normally relative to local axes, transform up
		if (parent_)
		{
			Quaternion qi = inverse( parent_->_getDerivedOrientation() );
			orientation_ = orientation_ * qi * q * _getDerivedOrientation();
		} else {

		}
		break;
	case TS_PARENT:
		// Rotations are normally relative to local axes, transform up
		orientation_ = q * orientation_;
		break;
	}
	needUpdate();
}

}
