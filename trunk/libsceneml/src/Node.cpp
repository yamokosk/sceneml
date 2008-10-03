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

#include <Variable.h>
#include <math/Util.h>

#include <boost/cast.hpp>
#include <iostream>

namespace TinySG {

using namespace log4cxx;

// Initialize static elements
LoggerPtr Node::logger(Logger::getLogger("TinySG.Node"));
Node::QueuedUpdates Node::queuedUpdates_;
const std::string Node::ObjectTypeID("TinySG_Node");

// Class implementation
Node::Node () :
	parent_(NULL),
	flags_(0)
{
	//needUpdate();
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
	ObjectMap::iterator itr;
	Entity* ret;
	for ( itr = sceneObjects_.begin(); itr != sceneObjects_.end(); itr++ )
	{
		ret = itr->second;
		ret->_notifyAttached((Node*)0);
	}
	sceneObjects_.clear();
}

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
}

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
	} else
		return NULL;
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
		cancelUpdate(ret);

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
	{
		ChildNodeIterator i = children_.find(child->getName());
		// ensure it's our child
		if (i != children_.end() && i->second == child)
		{
			// cancel any pending update
			cancelUpdate(child);

			children_.erase(i);
			child->setParent(NULL);
		}
	}
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
	cancelUpdate(ret);

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
	childrenToUpdate_.clear();
}

//! Gets the parent of this SceneNode.
Node* Node::getParent(void) const
{
	return parent_;
}



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
}

void Node::setParent(Node *parent)
{
	bool different = (parent != parent_);

	parent_ = parent;
	// Request update from parent
	parentNotified_ = false ;
	needUpdate();

	// Call listener (note, only called if there's something to do)
	/*if (mListener && different)
	{
		if (parent_)
		mListener->nodeAttached(this);
		else
		mListener->nodeDetached(this);
	}*/
}

//! Triggers the node to update it's combined transforms.
void Node::_updateFromParent (void)
{
	updateFromParentImpl();
}



Object* NodeFactory::createInstanceImpl(const std::string& name, const PropertyCollection* params = 0)
{

}

void NodeFactory::destroyInstance(Object* obj)
{

}

} // Namespace: TinySG

ostream& operator << (ostream& os, TinySG::Node& s)
{
	return os << "Name: " << s.getName() << std::endl
	   << "Tmatrix: " << std::endl << s._getFullTransform() << std::endl;
}

