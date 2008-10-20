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

#include "Node.h"

#include <math/Util.h>
#include <boost/cast.hpp>
#include <iostream>

namespace TinySG {

using namespace log4cxx;

// Initialize static elements
LoggerPtr Node::logger(Logger::getLogger("TinySG.Node"));
Node::QueuedUpdates Node::queuedUpdates_;
Node::NodesToUpdate Node::nodesToUpdate_;
const std::string Node::ObjectTypeID("TinySG_Node");

// Class implementation
Node::Node () :
	parent_(NULL),
	level_(0),
	orientation_( QuaternionFactory::IDENTITY ),
	position_( VectorFactory::Vector3( ZERO ) ),
	scale_( VectorFactory::Vector3( ONES ) ),
	derivedOrientation_( QuaternionFactory::IDENTITY ),
	derivedPosition_( VectorFactory::Vector3( ZERO ) ),
	derivedScale_( VectorFactory::Vector3( ONES ) ),
	cachedTransform_( MatrixFactory::Matrix4x4( IDENTITY ) ),
	flags_(Node::NeedParentUpdateBit | Node::CachedTransformOutOfDateBit)
{
	children_.clear();
}

Node::~Node()
{
	removeAllChildren();
	if (parent_)
		parent_->removeChild(this);

	// Remove ourselves from the update queue
	dequeueForUpdate(this);

	// Detach all objects, do this manually to avoid needUpdate() call
	// which can fail because of deleted items
	/*ObjectMap::iterator itr;
	Entity* ret;
	for ( itr = sceneObjects_.begin(); itr != sceneObjects_.end(); itr++ )
	{
		ret = itr->second;
		ret->_notifyAttached((Node*)0);
	}
	sceneObjects_.clear();*/
}

Object* Node::clone() const
{
	return new Node(*this);
}

bool Node::operator< (const Node& n)
{
	return (this->getLevel() < n.getLevel());
}
/*
//! Gets the orientation of the node as derived from all parents.
Quaternion Node::_getDerivedOrientation (void)
{
	if (needParentUpdate_)
	{
		_updateFromParent();
	}
	return derivedOrientation_;
}

//! Gets the position of the node as derived from all parents.
const ColumnVector& Node::_getDerivedPosition (void)
{
	if (needParentUpdate_)
	{
		_updateFromParent();
	}
	//ColumnVector ret(derivedPosition_);
	//ret.Release();
	return derivedPosition_;
}

//! Gets the scaling factor of the node as derived from all parents.
const ColumnVector& Node::_getDerivedScale (void)
{
	if (needParentUpdate_)
	{
		_updateFromParent();
	}
	return derivedScale_;
}

//! Gets the full transformation matrix for this node.
const Matrix& Node::_getFullTransform (void)
{
	if (cachedTransformOutOfDate_)
	{
		// Use derived values
		cachedTransform_.SubMatrix(1,3,4,4) = _getDerivedPosition();

		Matrix m = MatrixFactory::FromQuaternion( _getDerivedOrientation() );
		cachedTransform_.SubMatrix(1,3,1,3) = m;
		cachedTransformOutOfDate_ = false;
	}
	return cachedTransform_;
}

const Matrix& Node::getFullTransform (void) const
{
	return cachedTransform_;
}*/

//! Adds a (precreated) child scene node to this node.
void Node::addChild (Node* child)
{
	if (child->parent_)
	{
		SML_EXCEPT(Exception::ERR_INVALIDPARAMS,
			"Node '" + child->getName() + "' already was a child of '" + child->parent_->getName() + "'.");
	}

	children_[child->getName()] = child;
	child->setParent(this);
	Node::queueForUpdate(child);
}

//! Reports the number of child nodes under this one.
unsigned short Node::numChildren (void) const
{
	return static_cast< unsigned short >( children_.size() );
}

//! Gets a pointer to a child node.
Node* Node::getChild (unsigned short index) const
{
	if( index < children_.size() )
	{
		ConstChildNodeIterator i = children_.begin();
		while (index--) ++i;
		return i->second;
	} else {
		SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Child index out of bounds.");
	}
}

//! Gets a pointer to a named child node.
Node* Node::getChild (const std::string& name) const
{
	ConstChildNodeIterator i = children_.find(name);

	if (i == children_.end())
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Child node named " + name + " does not exist.");
	}
	return i->second;
}

//! Drops the specified child from this node.
Node* Node::removeChild (unsigned short index)
{
	Node* ret;
	if (index < children_.size())
	{
		ChildNodeIterator i = children_.begin();
		while (index--) ++i;
		ret = i->second;

		// cancel any pending update
		//cancelUpdate(ret);

		children_.erase(i);
		ret->setParent(NULL);
		return ret;
	}
	else
	{
		SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Child index out of bounds.");
	}
	return 0;
}

//! Drops the specified child from this node.
Node* Node::removeChild (Node *child)
{
	if (child)
		removeChild( child->getName() );
	return child;
}

//! Drops the named child from this node.
Node* Node::removeChild (const std::string &name)
{
	ChildNodeIterator i = children_.find(name);

	if (i == children_.end())
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Child node named " + name + " does not exist.");
	}

	Node* ret = i->second;
	// Cancel any pending update
	//cancelUpdate(ret);

	children_.erase(i);
	ret->setParent(NULL);

	return ret;
}

//! Removes all child Nodes attached to this node.
void Node::removeAllChildren (void)
{
	ChildNodeMap::iterator i, iend;
	iend = children_.end();
	for (i = children_.begin(); i != iend; ++i)
	{
		i->second->setParent(0);
	}
	children_.clear();
	//childrenToUpdate_.clear();
}

//! Gets the parent of this SceneNode.
Node* Node::getParent(void) const
{
	return parent_;
}



void Node::setOrientation (const Quaternion &q)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );
	orientation_ = q;
	setFlags( getFlags() & Node::CachedTransformOutOfDateBit );
}

void Node::setOrientation (Real w, Real x, Real y, Real z)
{
	Quaternion q(w, x, y, z);
	setOrientation(q);
}

void Node::setPosition(const ColumnVector &pos)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );
	position_ = pos;
	setFlags( getFlags() & Node::CachedTransformOutOfDateBit );
}

void Node::setPosition(Real x, Real y, Real z)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );
	ColumnVector v(3); v << x << y << z;
	setPosition(v);
}

void Node::setScale(const ColumnVector &s)
{
	scale_ = s;
	setFlags( getFlags() & Node::CachedTransformOutOfDateBit );
}

void Node::setScale(Real x, Real y, Real z)
{
	ColumnVector s(3); s << x << y << z;
	setScale(s);
}

/*
ReturnMatrix Node::getLocalAxes (void) const
{
	ColumnVector axisX = VectorFactory::Vector3( UNIT_X );
	axisX = orientation_ * axisX;

	ColumnVector axisY = VectorFactory::Vector3( UNIT_Y );
	axisY = orientation_ * axisY;

	ColumnVector axisZ = VectorFactory::Vector3( UNIT_Z );
	axisZ = orientation_ * axisZ;

	// Concatenate columns to form matrix
	Matrix ret = MatrixFactory::Matrix4x4( IDENTITY );
	ret.SubMatrix(1,3,1,3) = (axisX & axisY & axisZ);
	ret.Release(); return ret;
}*/

const Matrix& Node::getFullTransform()
{
	if ( queryFlag( Node::CachedTransformOutOfDateBit ) )
	{
		LOG4CXX_TRACE(logger, "Cached transform out of date. Calling update method.");
		updateCachedTransform();
	} else {
		LOG4CXX_TRACE(logger, "Cached transform is OK.");
	}
	return cachedTransform_;
}

void Node::updateCachedTransform()
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );

	cachedTransform_ = RotFromQuaternion( getOrientation() );
	cachedTransform_.SubMatrix(1,3,4,4) = getPosition();
	setFlags( getFlags() & !Node::CachedTransformOutOfDateBit );
}

const Quaternion& Node::getParentOrientation() const
{
	return parent_->getDerivedOrientation();
}

const ColumnVector& Node::getParentPosition() const
{
	return parent_->getDerivedPosition();
}

const ColumnVector& Node::getParentScale() const
{
	return parent_->getDerivedScale();
}

const Quaternion& Node::getDerivedOrientation()
{
	if ( getFlags() & Node::NeedParentUpdateBit )
	{
		updateFromParent();
	}
	return derivedOrientation_;
}

const ColumnVector& Node::getDerivedPosition()
{
	if ( getFlags() & Node::NeedParentUpdateBit )
	{
		updateFromParent();
	}
	return derivedPosition_;
}

const ColumnVector& Node::getDerivedScale()
{
	if ( getFlags() & Node::NeedParentUpdateBit )
	{
		updateFromParent();
	}
	return derivedScale_;
}

void Node::dequeueForUpdate(Node *n)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ << " for node \"" << n->getName() << "\"." );

	//if ( n->queryFlag( Node::QueuedForUpdateBit ) )
	//{
		// Unset the queued for update flag
	//	n->setFlags( n->getFlags() & !Node::QueuedForUpdateBit );

		// Erase from queued updates
		QueuedUpdates::iterator it = std::find( queuedUpdates_.begin(), queuedUpdates_.end(), n );
		//assert( it != queuedUpdates_.end() );
		if ( it != queuedUpdates_.end() )
		{
			// Optimised algorithm to erase an element from unordered vector.
			*it = queuedUpdates_.back();
			queuedUpdates_.pop_back();
		}
	//}
}

void Node::queueForUpdate(Node *n)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ << " for node \"" << n->getName() << "\"." );

	QueuedUpdates::iterator it = std::find( queuedUpdates_.begin(), queuedUpdates_.end(), n );
	if ( it == queuedUpdates_.end() )
	{
		LOG4CXX_TRACE(logger, "Queued node \"" <<  n->getName() << "\"." );
		queuedUpdates_.push_back(n);
	}
	// Don't queue the node more than once
	/*if ( n->queryFlag( !Node::QueuedForUpdateBit ) )
	{
		// Set the queued for update flag
		n->setFlags( n->getFlags() & Node::QueuedForUpdateBit );

		//queuedUpdates_.push(n);
		std::cout << n->getName() << " queued for update." << std::endl;
	}*/
}

void Node::processQueuedUpdates (void)
{
	// Builds the priority queue
	QueuedUpdates::iterator iter;
	for (iter = queuedUpdates_.begin(); iter != queuedUpdates_.end(); ++iter)
	{
		Node* n = (*iter);
		n->needUpdate();
	}
	queuedUpdates_.clear();
	/*
	while ( !queuedUpdates_.empty() )
	{
		Node* n = queuedUpdates_.front();
		queuedUpdates_.pop();

		if ( !n->queryFlag( Node::QueuedForUpdateBit ) )
		{
			continue; //LOG4CXX_ERROR(logger, "Mismatch between node \"" << n->getName() << "\" queued flag and its presence in the update queue!" );
		} else {
			LOG4CXX_TRACE(logger, "Processing updates for node \"" << n->getName() << "\"." );
		}
		// Update, and force parent update since chances are we've ended
		// up with some mixed state in there due to re-entrancy
		// Unset the queued for update flag
		n->setFlags( n->getFlags() & !Node::QueuedForUpdateBit );
		n->needUpdate();
	}*/
	//LOG4CXX_DEBUG(logger, "All queued node updates processed and queue cleared." );
}

void Node::updateFromParent()
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ << " for node \"" << getName() << ", " << getLevel() << "\"." );

	if ( hasParent() )
	{
		// Set level
		level_ = getParent()->getLevel() + 1;

		if ( queryFlag( Node::OrientationInheritedBit ) ) {
			// Combine orientation with that of parent
			derivedOrientation_ = getParentOrientation() * orientation_;
		} else {
			// No inheritance
			derivedOrientation_ = orientation_;
		}

		// Update scale
		if ( queryFlag( Node::ScaleInheritedBit ) ) {
			// Scale own position by parent scale, NB just combine
			// as equivalent axes, no shearing
			//derivedScale_ = parentScale * scale_;
			derivedScale_ = getParentScale();
		} else {
			// No inheritance
			derivedScale_ = scale_;
		}

		// Change position vector based on parent's orientation & scale
		//derivedPosition_ = parentOrientation * (parentScale_ * position_);
		derivedPosition_ = getParentOrientation() * position_;

		// Add altered position vector to parents
		derivedPosition_ += getParentPosition();
	} else {
		level_ = 0;

		// Root node, no parent
		derivedOrientation_ = orientation_;
		derivedPosition_ = position_;
		derivedScale_ = scale_;
	}

	setFlags( getFlags() & ( Node::CachedTransformOutOfDateBit | !Node::NeedParentUpdateBit ) );
}

void Node::needUpdate()
{
	//setFlags( getFlags() & (Node::NeedParentUpdateBit | Node::CachedTransformOutOfDateBit) );

	addToUpdateQueue(this);

	// Make sure we're not root and parent hasn't been notified before
	ChildNodeMap::iterator i, iend;
	iend = children_.end();
	for (i = children_.begin(); i != iend; ++i)
	{
		i->second->needUpdate();
	}
}

void Node::addToUpdateQueue(Node* n)
{
	nodesToUpdate_.push(n);
}

void Node::updateNodes(void)
{
	while ( !nodesToUpdate_.empty() )
	{
		Node* n = nodesToUpdate_.top();
		nodesToUpdate_.pop();

		n->updateFromParent();
	}
}

/*
//! Sets the current transform of this node to be the 'initial state' ie that position / orientation / scale to be used as a basis for delta values used in keyframe animation.
void Node::setInitialState (void)
{
	initialPosition_ = position_;
	initialOrientation_ = orientation_;
	initialScale_ = scale_;
}

//! Resets the position / orientation / scale of this node to it's initial state, see setInitialState for more info.
void Node::resetToInitialState (void)
{
	position_ = initialPosition_;
	orientation_ = initialOrientation_;
	scale_ = initialScale_;

	needUpdate();
}

//! Gets the initial position of this node, see setInitialState for more info.
const ColumnVector& Node::getInitialPosition (void) const
{
	return initialPosition_;
}

//! Gets the initial orientation of this node, see setInitialState for more info.
const Quaternion& Node::getInitialOrientation (void) const
{
	return initialOrientation_;
}

//! Gets the initial position of this node, see setInitialState for more info.
const ColumnVector& Node::getInitialScale (void) const
{
	return initialScale_;
}

//! To be called in the event of transform changes to this node that require it's recalculation.
void Node::needUpdate (bool forceParentUpdate)
{
	std::cout << getName() << "::needUpdate()" << std::endl;

	needParentUpdate_ = true;
	needChildUpdate_ = true;
	cachedTransformOutOfDate_ = true;

	// Make sure we're not root and parent hasn't been notified before
	if (parent_ && (!parentNotified_ || forceParentUpdate))
	{
		parent_->requestUpdate(this, forceParentUpdate);
		parentNotified_ = true ;
	}

	// all children will be updated
	childrenToUpdate_.clear();
}

//! Called by children to notify their parent that they need an update.
void Node::requestUpdate (Node *child, bool forceParentUpdate)
{
	// If we're already going to update everything this doesn't matter
	if (needChildUpdate_)
	{
		return;
	}

	childrenToUpdate_.insert(child);
	// Request selective update of me, if we didn't do it before
	if (parent_ && (!parentNotified_ || forceParentUpdate))
	{
		parent_->requestUpdate(this, forceParentUpdate);
		parentNotified_ = true ;
	}
}

//! Called by children to notify their parent that they no longer need an update.
void Node::cancelUpdate (Node *child)
{
	childrenToUpdate_.erase(child);

	// Propogate this up if we're done
	if (childrenToUpdate_.empty() && parent_ && !needChildUpdate_)
	{
		parent_->cancelUpdate(this);
		parentNotified_ = false ;
	}
}

//! Internal method to update the Node.
void Node::_update (bool updateChildren, bool parentHasChanged)
{
	std::cout << getName() << "::_update()" << std::endl;

	// always clear information about parent notification
	parentNotified_ = false ;

	// Short circuit the off case
	if (!updateChildren_ && !needParentUpdate_ && !needChildUpdate_ && !parentHasChanged_ )
	{
		return;
	}

	// See if we should process everyone
	if (needParentUpdate_ || parentHasChanged_)
	{
		// Update transforms from parent
		_updateFromParent();
	}

	if (needChildUpdate_ || parentHasChanged_)
	{
		ChildNodeMap::iterator it, itend;
		itend = children_.end();
		for (it = children_.begin(); it != itend; ++it)
		{
			Node* child = it->second;
			child->_update(true, true);
		}
		childrenToUpdate_.clear();
	}
	else
	{
		// Just update selected children
		ChildUpdateSet::iterator it, itend;
		itend = childrenToUpdate_.end();
		for(it = childrenToUpdate_.begin(); it != itend; ++it)
		{
			Node* child = *it;
			child->_update(true, false);
		}

		childrenToUpdate_.clear();
	}

	needChildUpdate_ = false;
}

void Node::dequeueForUpdate(Node *n)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );

	// Don't queue the node more than once
	if ( n->queuedForUpdate() )
	{
		n->flags_ ^= Node::QueuedForUpdateMask;

		// Erase from queued updates
		QueuedUpdates::iterator it = std::find( queuedUpdates_.begin(), queuedUpdates_.end(), this );
		assert( it != queuedUpdates_.end() );
		if ( it != queuedUpdates_.end() )
		{
			// Optimised algorithm to erase an element from unordered vector.
			*it = queuedUpdates_.back();
			queuedUpdates_.pop_back();
		}
	}
}

void Node::queueForUpdate(Node *n)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );

	// Don't queue the node more than once
	if (!n->queuedForUpdate())
	{
		n->flags_ ^= Node::QueuedForUpdateMask;
		queuedUpdates_.push_back(n);
		std::cout << n->getName() << " queued for update." << std::endl;
	}
}

inline
bool Node::queuedForUpdate()
{
	return (Node::QueuedForUpdateMask & flags_);
}

//! Process queued 'needUpdate' calls.
void Node::processQueuedUpdates(void)
{
	for (QueuedUpdates::iterator i = queuedUpdates_.begin(); i != queuedUpdates_.end(); ++i)
	{
		if ( !n->queuedForUpdate() )
		{
			LOG4CXX_ERROR(logger, "Mismatch between node \"" << n->getName() << "\" queued flag and its presence in the update queue!" );
		}
		// Update, and force parent update since chances are we've ended
		// up with some mixed state in there due to re-entrancy
		Node* n = *i;
		n->flags_ ^= Node::QueuedForUpdateMask;
		n->needUpdate(true);
	}
	queuedUpdates_.clear();
	LOG4CXX_DEBUG(logger, "All queued node updates processed and queue cleared." );
}

void Node::updateFromParentImpl(void)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ << " for node \"" << getName() << "\"." );

	if (parent_)
	{
		// Update orientation
		std::cout << "	update orientation..." << std::endl;
		Quaternion parentOrientation = parent_->_getDerivedOrientation();
		if (inheritOrientation_)
		{
			// Combine orientation with that of parent
			derivedOrientation_ = parentOrientation * orientation_;
		}
		else
		{
			// No inheritence
			derivedOrientation_ = orientation_;
		}

		// Update scale
		std::cout << "	update scale..." << std::endl;
		const ColumnVector& parentScale = parent_->_getDerivedScale();
		if (inheritScale_)
		{
			// Scale own position by parent scale, NB just combine
			// as equivalent axes, no shearing
			//derivedScale_ = parentScale * scale_;
			derivedScale_ = parentScale;
		}
		else
		{
			// No inheritence
			derivedScale_ = scale_;
		}

		// Change position vector based on parent's orientation & scale
		std::cout << "	update position..." << std::endl;
		//derivedPosition_ = parentOrientation * (parentScale_ * position_);
		derivedPosition_ = parentOrientation * position_;

		// Add altered position vector to parents
		derivedPosition_ += parent_->_getDerivedPosition();
	}
	else
	{
		// Root node, no parent
		derivedOrientation_ = orientation_;
		derivedPosition_ = position_;
		derivedScale_ = scale_;
	}

	cachedTransformOutOfDate_ = true;
	needParentUpdate_ = false;

	// Notify objects that it has been moved
	ObjectMapConstIterator i;
	for (i = sceneObjects_.begin(); i != sceneObjects_.end(); ++i)
	{
		Entity* object = i->second;
		object->_notifyMoved();
	}

}

Node*  Node::createChildImpl(void)
{
	if ( !getManager() )
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Node named " + this->getName() + " does not belong to a manager.");
	}
	return getManager()->createNode();
}

Node* Node::createChildImpl(const std::string& name)
{
	if ( !getManager() )
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Node named " + this->getName() + " does not belong to a manager.");
	}
	return dynamic_cast<Node*>( getManager()->createObject(name, Node::ObjectTypeID) );
}*/

void Node::setParent(Node *parent)
{
	bool different = (parent != parent_);

	parent_ = parent;

	if ( parent != NULL )
		level_ = parent->getLevel() + 1;
	else
		level_ = 0;
	//level_ = parent->getLevel() + 1;
	// Request update from parent
	//parentNotified_ = false ;
	if (different)
		Node::queueForUpdate(this);
}

/*
//! Triggers the node to update it's combined transforms.
void Node::_updateFromParent (void)
{
	updateFromParentImpl();
}*/



Object* NodeFactory::createInstanceImpl(const PropertyCollection* params)
{
	return (new Node);
}

void NodeFactory::destroyInstance(Object* obj)
{
	delete obj;
}

} // Namespace: TinySG

/*
ostream& operator << (ostream& os, TinySG::Node& s)
{
	return os << "Name: " << s.getName() << std::endl
	   << "Tmatrix: " << std::endl << s._getFullTransform() << std::endl;
}*/

