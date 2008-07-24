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
#include <boost/cast.hpp>
#include <Variable.h>
#include <iostream>

namespace sml {

	unsigned long Node::nextGeneratedNameExt_ = 1;
	Node::QueuedUpdates Node::queuedUpdates_;

	Node::Node (SceneMgr *mgr) :
		manager_(mgr),
		//worldAABB_(),
		parent_(NULL),
		//childrenToUpdate_(false),
		needParentUpdate_(false),
		needChildUpdate_(false),
		parentNotified_(false),
		queuedForUpdate_(false),
		name_(""),
		orientation_( math::QuaternionFactory::Quat( math::IDENTITY ) ),
		position_( math::VectorFactory::Vector3( math::ZERO ) ),
		scale_( math::VectorFactory::Vector3( math::ONES ) ),
		inheritOrientation_(true),
		inheritScale_(true),
		derivedOrientation_( math::QuaternionFactory::Quat( math::IDENTITY ) ),
		derivedPosition_( math::VectorFactory::Vector3( math::ZERO ) ),
		derivedScale_( math::VectorFactory::Vector3( math::ONES ) ),
		initialPosition_( math::VectorFactory::Vector3( math::ZERO ) ),
		initialOrientation_( math::QuaternionFactory::Quat( math::IDENTITY ) ),
		initialScale_( math::VectorFactory::Vector3( math::ONES ) ),
		cachedTransform_( math::MatrixFactory::Matrix4x4( math::IDENTITY ) ),
		cachedTransformOutOfDate_(true)
	{
		// Generate a name
		std::stringstream str;
		str << "Unnamed_" << nextGeneratedNameExt_++;
		name_ = str.str();

		//needUpdate();
	}

	//! Constructor, only to be called by the creator SceneManager.
	Node::Node(SceneMgr* mgr, const std::string& name) :
		manager_(mgr),
		//worldAABB_(),
		parent_(NULL),
		//childrenToUpdate_(false),
		needParentUpdate_(false),
		needChildUpdate_(false),
		parentNotified_(false),
		queuedForUpdate_(false),
		name_(name),
		orientation_( math::QuaternionFactory::Quat( math::IDENTITY ) ),
		position_( math::VectorFactory::Vector3( math::ZERO ) ),
		scale_( math::VectorFactory::Vector3( math::ONES ) ),
		inheritOrientation_(true),
		inheritScale_(true),
		derivedOrientation_( math::QuaternionFactory::Quat( math::IDENTITY ) ),
		derivedPosition_( math::VectorFactory::Vector3( math::ZERO ) ),
		derivedScale_( math::VectorFactory::Vector3( math::ONES ) ),
		initialPosition_( math::VectorFactory::Vector3( math::ZERO ) ),
		initialOrientation_( math::QuaternionFactory::Quat( math::IDENTITY ) ),
		initialScale_( math::VectorFactory::Vector3( math::ONES ) ),
		cachedTransform_( math::MatrixFactory::Matrix4x4( math::IDENTITY ) ),
		cachedTransformOutOfDate_(true)
	{
	}

	Node::~Node()
	{
		removeAllChildren();
		if (parent_)
			parent_->removeChild(this);

		if (queuedForUpdate_)
		{
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

		// Detach all objects, do this manually to avoid needUpdate() call
		// which can fail because of deleted items
		ObjectMap::iterator itr;
		SceneObject* ret;
		for ( itr = sceneObjects_.begin(); itr != sceneObjects_.end(); itr++ )
		{
			ret = itr->second;
			ret->_notifyAttached((Node*)0);
		}
		sceneObjects_.clear();

		/*if (mWireBoundingBox) {
			delete mWireBoundingBox;
		}*/
	}

	void Node::update(Subject* sub, int hint)
	{
		std::cout << "Got that a variable I was watching has changed." << std::endl;

		Variable* var = boost::polymorphic_downcast<Variable*>(sub);

		std::string type = var->getType(); //type of variable we are listening to

		if ( !type.compare("translation") )
		{
			ColumnVector transl = var->getVector();
			std::cout << "Translating " << transl.t() << std::endl;
			this->translate(transl, TS_PARENT);
		} else if ( !type.compare("rotation") ) {

			std::string subtype = var->getSubType(); //type of variable we are listening to

			if ( !subtype.compare("x") )
			{
				Real angle = var->getScalar();
				ColumnVector axis = math::VectorFactory::Vector3( math::UNIT_X );
				this->rotate(axis, angle, TS_PARENT);
			}
			else if ( !subtype.compare("y") ) {
				Real angle = var->getScalar();
				ColumnVector axis = math::VectorFactory::Vector3( math::UNIT_Y );
				this->rotate(axis, angle, TS_PARENT);
			}
			else if ( !subtype.compare("z") ) {
				Real angle = var->getScalar();
				ColumnVector axis = math::VectorFactory::Vector3( math::UNIT_Z );
				this->rotate(axis, angle, TS_PARENT);
			}
			else if ( !subtype.compare("e123") ) {
				ColumnVector angles = var->getVector();
				math::Quaternion q = math::QuaternionFactory::FromEulerAngles( math::E123, angles);
				this->rotate(q, TS_PARENT);
			}
			else if ( !subtype.compare("t123") ) {
				//dTFromEuler123(tmatrix_, -(data_.get())[0], -(data_.get())[1], -(data_.get())[2]);
			}
			else {
				SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Unknown variable subtype: " + subtype);
			}
			//std::cout << "Rotating " << angle << " about " << axis.t() << std::endl;

		} else {
			SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Unknown variable type: " + type);
		}
	}

	void Node::attachObject (SceneObject *obj)
	{
		if (obj->isAttached())
		{
			SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Object already attached to a Node.");
		}

		obj->_notifyAttached(this);

		// Also add to name index
		std::pair<ObjectMap::iterator, bool> insresult =
		sceneObjects_.insert(ObjectMap::value_type(obj->getName(), obj));
		assert(insresult.second && "Object was not attached because an object of the "
				"same name was already attached to this node.");

		// Make sure bounds get updated (must go right to the top)
		needUpdate();
	}

	unsigned short Node::numAttachedObjects (void) const
	{
		return static_cast< unsigned short >( sceneObjects_.size() );
	}

	//! Retrieves a pointer to an attached object.
	SceneObject* Node::getAttachedObject (unsigned short index)
	{
		if (index < sceneObjects_.size())
		{
			ObjectMap::iterator i = sceneObjects_.begin();
			// Increment (must do this one at a time)
			while (index--)++i;

			return i->second;
		}
		else
		{
			SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Object index out of bounds.");
		}
		return 0;
	}

	//! Retrieves a pointer to an attached object.
	SceneObject* Node::getAttachedObject (const std::string &name)
	{
		// Look up
		ObjectMap::iterator i = sceneObjects_.find(name);

		if (i == sceneObjects_.end())
		{
			SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Attached object " + name + " not found.");
		}

		return i->second;
	}

	//! Detaches the indexed object from this scene node.
	SceneObject* Node::detachObject (unsigned short index)
	{
		SceneObject* ret;
		if (index < sceneObjects_.size())
		{

			ObjectMap::iterator i = sceneObjects_.begin();
			// Increment (must do this one at a time)
			while (index--)++i;

			ret = i->second;
			sceneObjects_.erase(i);
			ret->_notifyAttached((Node*)0);

			// Make sure bounds get updated (must go right to the top)
			needUpdate();

			return ret;

		}
		else
		{
			SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Object index out of bounds.");
		}
		return 0;
	}

	//! Detaches an object by pointer.
	void Node::detachObject (SceneObject *obj)
	{
		ObjectMap::iterator i, iend;
		iend = sceneObjects_.end();
		for (i = sceneObjects_.begin(); i != iend; ++i)
		{
			if (i->second == obj)
			{
				sceneObjects_.erase(i);
				break;
			}
		}
		obj->_notifyAttached((Node*)0);

		// Make sure bounds get updated (must go right to the top)
		needUpdate();
	}

	//! Detaches the named object from this node and returns a pointer to it.
	SceneObject* Node::detachObject (const std::string &name)
	{
		ObjectMapIterator it = sceneObjects_.find(name);
		if (it == sceneObjects_.end())
		{
			SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Object " + name + " is not attached "
					"to this node.");
		}
		SceneObject* ret = it->second;
		sceneObjects_.erase(it);
		ret->_notifyAttached((Node*)0);
		// Make sure bounds get updated (must go right to the top)
		needUpdate();

		return ret;
	}

	//! Detaches all objects attached to this node.
	void Node::detachAllObjects (void)
	{
		ObjectMapIterator itr;
		SceneObject* ret;
		for ( itr = sceneObjects_.begin(); itr != sceneObjects_.end(); itr++ )
		{
			ret = itr->second;
			ret->_notifyAttached((Node*)0);
		}
		sceneObjects_.clear();
		// Make sure bounds get updated (must go right to the top)
		needUpdate();
	}

	//! Internal method to update the Node.
	void Node::_update (bool updateChildren, bool parentHasChanged)
	{
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

/*	//! Tells the SceneNode to update the world bound info it stores.
	void Node::_updateBounds (void)
	{
		// Reset bounds first
		mWorldAABB.setNull();

		// Update bounds from own attached objects
		ObjectMap::iterator i;
		for (i = sceneObjects_.begin(); i != sceneObjects_.end(); ++i)
		{
			// Merge world bounds of each object
			mWorldAABB.merge(i->second->getWorldBoundingBox(true));
		}

		// Merge with children
		ChildNodeMap::iterator child;
		for (child = children_.begin(); child != children_.end(); ++child)
		{
			SceneNode* sceneChild = static_cast<SceneNode*>(child->second);
			mWorldAABB.merge(sceneChild->mWorldAABB);
		}
	}

	//! Gets the axis-aligned bounding box of this node (and hence all subnodes).
	const AxisAlignedBox& Node::_getWorldAABB (void) const
	{
		return mWorldAABB;
	}

	//! Retrieves an iterator which can be used to efficiently step through the objects attached to this node.
	ObjectIterator Node::getAttachedObjectIterator (void)
	{
		return ObjectIterator(sceneObjects_.begin(), sceneObjects_.end());
	}

	//! Retrieves an iterator which can be used to efficiently step through the objects attached to this node.
	ConstObjectIterator Node::getAttachedObjectIterator (void) const
	{
		return ConstObjectIterator(sceneObjects_.begin(), sceneObjects_.end());
	}
	*/
	//! Gets the creator of this scene node.
	SceneMgr* Node::getManager (void) const
	{
		return manager_;
	}

	//! Gets the parent of this SceneNode.
	Node* Node::getParent(void) const
	{
		return parent_;
	}

	//! Returns the name of the node.
	const std::string& Node::getName (void) const
	{
		return name_;
	}

	//void attachObject(SceneObject* obj);
	// Must call this->notify() whenever node's transformation changes

	//! Returns a quaternion representing the nodes orientation.
	const math::Quaternion& Node::getOrientation () const
	{
		return orientation_;
	}

	//! Sets the orientation of this node via a quaternion.
	void Node::setOrientation (const math::Quaternion &q)
	{
		orientation_ = q;
		needUpdate();
	}

	//! Sets the orientation of this node via quaternion parameters.
	void Node::setOrientation (math::Real w, math::Real x, math::Real y, math::Real z)
	{
		math::Quaternion q(w, x, y, z);
		setOrientation(q);
		needUpdate();
	}

	//! Resets the nodes orientation (local axes as world axes, no rotation).
	void Node::resetOrientation (void)
	{
		setOrientation( math::QuaternionFactory::Quat( math::IDENTITY ) );
		needUpdate();
	}

	//! Sets the position of the node relative to it's parent.
	void Node::setPosition(const ColumnVector &pos)
	{
		position_ = pos;
		needUpdate();
	}

	//! Sets the position of the node relative to it's parent.
	void Node::setPosition(math::Real x, math::Real y, math::Real z)
	{
		ColumnVector v; v << x << y << z;
		setPosition(v);
	}

	//! Gets the position of the node relative to it's parent.
	const ColumnVector& Node::getPosition(void) const
	{
		return position_;
	}

	//! Scales the node, combining it's current scale with the passed in scaling factor.
	//void Node::scale(const ColumnVector &scale);

	//! Scales the node, combining it's current scale with the passed in scaling factor.
	//void Node::scale(Real x, Real y, Real z);

	//! Moves the node along the cartesian axes.
	void Node::translate(const ColumnVector &d, TransformSpace relativeTo)
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
				math::Quaternion qi = math::inverse( parent_->_getDerivedOrientation() );
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
	void Node::translate(Real x, Real y, Real z, TransformSpace relativeTo)
	{
		ColumnVector v; v << x << y << z;
		translate(v, relativeTo);
	}

	//! Moves the node along arbitrary axes.
	void Node::translate(const SquareMatrix &axes, const ColumnVector &move, TransformSpace relativeTo)
	{
		ColumnVector derived = axes * move;
		translate(derived, relativeTo);
	}

	//! Moves the node along arbitrary axes.
	void Node::translate(const SquareMatrix &axes, Real x, Real y, Real z, TransformSpace relativeTo)
	{
		ColumnVector d(3); d << x << y << z;
		translate(axes,d,relativeTo);
	}

	//! Rotate the node around the Z-axis.
	/*void Node::roll(const Radian &angle, TransformSpace relativeTo=TS_LOCAL)
	{
		rotate(ColumnVector::UNIT_Z, angle, relativeTo);
	}

	//! Rotate the node around the X-axis.
	void Node::pitch(const Radian &angle, TransformSpace relativeTo=TS_LOCAL)
	{
		rotate(ColumnVector::UNIT_X, angle, relativeTo);
	}

	void Node::yaw(const Radian& angle, TransformSpace relativeTo)
	{
		rotate(ColumnVector::UNIT_Y, angle, relativeTo);
	}*/

	//! Rotate the node around an arbitrary axis.
	void Node::rotate(const ColumnVector &axis, math::Real angle, TransformSpace relativeTo)
	{
		math::Quaternion q = math::QuaternionFactory::FromAngleAxis(angle,axis);
		rotate(q, relativeTo);
	}

	//! Rotate the node around an aritrary axis using a Quarternion.
	void Node::rotate(const math::Quaternion &q, TransformSpace relativeTo)
	{
		switch(relativeTo)
		{
		case TS_PARENT:
			// Rotations are normally relative to local axes, transform up
			//orientation_ = q * orientation_;
			break;
		case TS_WORLD:
			// Rotations are normally relative to local axes, transform up
			//orientation_ = orientation_ * _getDerivedOrientation().Inverse() * q * _getDerivedOrientation();
			break;
		case TS_LOCAL:
			// Note the order of the mult, i.e. q comes after
			//orientation_ = orientation_ * q;
			break;
		}
		needUpdate();
	}

	//! Gets a matrix whose columns are the local axes based on the nodes orientation relative to it's parent.
	ReturnMatrix Node::getLocalAxes (void) const
	{
		ColumnVector axisX = math::VectorFactory::Vector3( math::UNIT_X );
		ColumnVector axisY = math::VectorFactory::Vector3( math::UNIT_Y );
		ColumnVector axisZ = math::VectorFactory::Vector3( math::UNIT_Z );

		axisX = orientation_ * axisX;
		axisY = orientation_ * axisY;
		axisZ = orientation_ * axisZ;

		// Concatenate columns to form matrix
		Matrix ret = axisX & axisY & axisZ;
		ret.Release();
		return ret;
	}

	//! Creates an unnamed new Node as a child of this node.
	Node* Node::createChild(const ColumnVector& translate, const math::Quaternion& rotate)
	{
		Node* newNode = createChildImpl();
		newNode->translate(translate);
		newNode->rotate(rotate);
		this->addChild(newNode);

		return newNode;
	}

	//! Creates a new named Node as a child of this node.
	Node* Node::createChild(const std::string& name, const ColumnVector& translate, const math::Quaternion& rotate)
	{
		Node* newNode = createChildImpl(name);
		newNode->translate(translate);
		newNode->rotate(rotate);
		this->addChild(newNode);

		return newNode;
	}

	//! Adds a (precreated) child scene node to this node.
	void Node::addChild (Node* child)
	{
		if (child->parent_)
		{
			SML_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Node '" + child->getName() + "' already was a child of '" + child->parent_->getName() + "'.");
		}

		children_.insert(ChildNodeMap::value_type(child->getName(), child));
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

	//! Retrieves an iterator for efficiently looping through all children of this node.
	/*Node::ChildNodeIterator Node::getChildIterator (void)
	{
		return ChildNodeIterator(children_.begin(), children_.end());
	}

	//! Retrieves an iterator for efficiently looping through all children of this node.
	Node::ConstChildNodeIterator Node::getChildIterator (void) const
	{
		return ConstChildNodeIterator(children_.begin(), children_.end());
	}*/

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

	//! Gets the orientation of the node as derived from all parents.
	math::Quaternion Node::_getDerivedOrientation (void)
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
	const Matrix& 	Node::_getFullTransform (void)
	{
		if (cachedTransformOutOfDate_)
		{
			// Use derived values
			/*cachedTransform_.makeTransform(
			_getDerivedPosition(),
			_getDerivedScale(),
			_getDerivedOrientation());*/
			cachedTransformOutOfDate_ = false;
		}
		return cachedTransform_;
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
	const math::Quaternion& Node::getInitialOrientation (void) const
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

	// Static Public Member Functions
	void Node::queueNeedUpdate(Node *n)
	{
		// Don't queue the node more than once
		if (!n->queuedForUpdate_)
		{
			n->queuedForUpdate_ = true;
			queuedUpdates_.push_back(n);
		}
	}

	//! Process queued 'needUpdate' calls.
	void Node::processQueuedUpdates (void)
	{
		for (QueuedUpdates::iterator i = queuedUpdates_.begin(); i != queuedUpdates_.end(); ++i)
		{
			// Update, and force parent update since chances are we've ended
			// up with some mixed state in there due to re-entrancy
			Node* n = *i;
			n->queuedForUpdate_ = false;
			n->needUpdate(true);
		}
		queuedUpdates_.clear();
	}

	void Node::updateFromParentImpl(void)
	{
		if (parent_)
		{
			// Update orientation
			math::Quaternion parentOrientation = parent_->_getDerivedOrientation();
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
			const ColumnVector& parentScale = parent_->_getDerivedScale();
			if (inheritScale_)
			{
				// Scale own position by parent scale, NB just combine
				// as equivalent axes, no shearing
				derivedScale_ = parentScale * scale_;
			}
			else
			{
				// No inheritence
				derivedScale_ = scale_;
			}

			// Change position vector based on parent's orientation & scale
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
			SceneObject* object = i->second;
			object->_notifyMoved();
		}

	}

	Node*  Node::createChildImpl(void)
	{
		if (!manager_)
		{
			SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Node named " + this->getName() + " does not belong to a manager.");
		}
        return manager_->createNode();
	}

	Node* Node::createChildImpl(const std::string& name)
	{
		if (!manager_)
		{
			SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Node named " + this->getName() + " does not belong to a manager.");
		}
        return manager_->createNode(name);
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
} // Namespace: sml
