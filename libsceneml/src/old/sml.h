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

#ifndef _SCENEML_SML_H_FILE_
#define _SCENEML_SML_H_FILE_

#include <string>

namespace sceneml {

	typedef	std::string		String;
	typedef	double 			Real;
	
	/************************************************************************/
	class Subject
	{
	public:
		typedef boost::signal<void (bool)>  signal_t;
		typedef boost::signals::connection  connection_t;

		Subject() {};
		
		connection_t connect(signal_t::slot_function_type subscriber)
		{
			return sig_.connect(subscriber);
		}

		void disconnect(connection_t subscriber)
		{
			subscriber.disconnect();
		}

		virtual void notify()
		{
			sig_(true);
		}

	private:
		signal_t sig_;
	};


	class Listener
	{
	public:
		Listener(Subject& m) : subject_(m)
		{
			connection_ = subject_.connect( boost::bind(&Observer::notify, this, _1) );
		}

		virtual ~Listener()
		{
			subject_.disconnect(connection_);
		}

		virtual void notify(bool bExtended) const = 0;

	protected:
		Subject& subject_;

	private:
		Subject::connection_t  connection_;
	};


	class Variable : public Subject
	{
	public:
		Variable(unsigned int n) : Subject(), storage_(n) {}
		Variable(const ColumnVector& data) : Subject(), storage_(data) {}
		
		// Set value
		void setValue(const ColumnVector& data) {storage_ = data; Subject::notify();}
		// Get value
		ReturnMatrix getValue() const 
		{
			ColumnVector ret(storage_);
			ret.release();
			return ret;
		} 
		
	private:
		ColumnVector storage_;
	};


	class SceneNode : public Subject
	{
	public:
		typedef HashMap< String, MovableObject * > 	ObjectMap
		typedef MapIterator< ObjectMap > 	ObjectIterator
		typedef ConstMapIterator<ObjectMap > 	ConstObjectIterator
		typedef HashMap< String, Node * > 	ChildNodeMap
		typedef MapIterator< ChildNodeMap > 	ChildNodeIterator
		typedef ConstMapIterator<ChildNodeMap > 	ConstChildNodeIterator
		// Enumeration denoting the spaces which a transform can be relative to. More...
		enum TransformSpace { TS_LOCAL, TS_PARENT, TS_WORLD }
		
	public:
		// Constructor, only to be called by the creator SceneManager.
		SceneNode (SceneManager *creator);
		// Constructor, only to be called by the creator SceneManager. 
		SceneNode (SceneManager *creator, const String &name)
		virtual ~SceneNode();
		
		SceneNode* getParent();
		SceneNode* createChildSceneNode();
		void attachObject(Entity* obj);
		
		// Returns a quaternion representing the nodes orientation.
		ReturnMatrix	getOrientation () const;
		// Sets the orientation of this node via a quaternion.
		void	setOrientation (const ColumnVector &q);
		// Sets the orientation of this node via quaternion parameters.
		void	setOrientation (Real w, Real x, Real y, Real z);
		// Sets the position of the node relative to it's parent.
		void 	setPosition (const ColumnVector &pos);
		// Sets the position of the node relative to it's parent.
		void 	setPosition (Real x, Real y, Real z);
		// Gets the position of the node relative to it's parent. 
		ReturnMatrix	getPosition (void) const;
		// Moves the node along the cartesian axes.
		void 	translate (const ColumnVector &d, TransformSpace relativeTo=TS_PARENT)
		// Moves the node along the cartesian axes.
		void 	translate (Real x, Real y, Real z, TransformSpace relativeTo=TS_PARENT)
		// Moves the node along arbitrary axes. 	
		void 	translate (const Matrix3 &axes, const ColumnVector &move, TransformSpace relativeTo=TS_PARENT)
		// Moves the node along arbitrary axes.
		void 	translate (const Matrix3 &axes, Real x, Real y, Real z, TransformSpace relativeTo=TS_PARENT)
		// Rotate the node around the Z-axis. 	
		void 	roll (const Radian &angle, TransformSpace relativeTo=TS_LOCAL)
		// Rotate the node around the X-axis. 	
		void 	pitch (const Radian &angle, TransformSpace relativeTo=TS_LOCAL)
		// Rotate the node around an arbitrary axis. 	
		void 	rotate (const ColumnVector &axis, const Radian &angle, TransformSpace relativeTo=TS_LOCAL)
		// Rotate the node around an aritrary axis using a Quarternion. 	
		void 	rotate (const Quaternion &q, TransformSpace relativeTo=TS_LOCAL)
		// Gets a matrix whose columns are the local axes based on the nodes orientation relative to it's parent. 	
		Matrix3 	getLocalAxes (void) const
		// Creates an unnamed new Node as a child of this node. 	
		Node * 	createChild (const ColumnVector &translate=ColumnVector::ZERO, const Quaternion &rotate=Quaternion::IDENTITY)
		// Creates a new named Node as a child of this node. 	
		Node * 	createChild (const String &name, const ColumnVector &translate=ColumnVector::ZERO, const Quaternion &rotate=Quaternion::IDENTITY)
		// Adds a (precreated) child scene node to this node. 	
		void 	addChild (Node *child)
		// Reports the number of child nodes under this one. 	
		unsigned short 	numChildren (void) const
		// Gets a pointer to a child node. 	
		Node * 	getChild (unsigned short index) const
		// Gets a pointer to a named child node. 	
		Node * 	getChild (const String &name) const
		// Retrieves an iterator for efficiently looping through all children of this node. 	
		ChildNodeIterator 	getChildIterator (void)
		// Retrieves an iterator for efficiently looping through all children of this node. 	
		ConstChildNodeIterator 	getChildIterator (void) const
		// Drops the specified child from this node. 	
		Node * 	removeChild (unsigned short index)
		// Drops the specified child from this node. 	
		Node * 	removeChild (Node *child)
		// Drops the named child from this node. 	
		Node * 	removeChild (const String &name)
		// Removes all child Nodes attached to this node. 	
		void 	removeAllChildren (void)
		// Gets the orientation of the node as derived from all parents. 	
		ReturnMatrix 	_getDerivedOrientation (void) const;
		// Gets the position of the node as derived from all parents. 	
		ReturnMatrix 	_getDerivedPosition (void) const;
		// Gets the scaling factor of the node as derived from all parents. 	
		ReturnMatrix 	_getDerivedScale (void) const;
		// Gets the full transformation matrix for this node. 	
		ReturnMatrix 	_getFullTransform (void) const;
		// Sets a listener for this Node. 	
		void 	setListener (Listener *listener)
		// Gets the current listener for this Node. 	
		Listener * 	getListener (void) const
		// Overridden from Renderable. 	
		const MaterialPtr & 	getMaterial (void) const
		// Overridden from Renderable. 	
		void 	getRenderOperation (RenderOperation &op)
		// Overridden from Renderable. 	
		void 	getWorldTransforms (Matrix &xform) const;
		ReturnMatrix 	getWorldOrientation (void) const;
		ReturnMatrix 	getWorldPosition (void) const;
		// Sets the current transform of this node to be the 'initial state' ie that position / orientation / scale to be used as a basis for delta values used in keyframe animation.
		void 	setInitialState (void)
		// Resets the position / orientation / scale of this node to it's initial state, see setInitialState for more info. 	
		void 	resetToInitialState (void)
		// Gets the initial position of this node, see setInitialState for more info. 	
		ReturnMatrix 	getInitialPosition (void) const
		// Gets the initial orientation of this node, see setInitialState for more info. 	
		ReturnMatrix 	getInitialOrientation (void) const
		// Gets the initial position of this node, see setInitialState for more info. 	
		ReturnMatrix 	getInitialScale (void) const
		// Overridden, see Renderable. 	
		Real 	getSquaredViewDepth (const Camera *cam) const
		// To be called in the event of transform changes to this node that require it's recalculation. 	
		void 	needUpdate (bool forceParentUpdate=false)
		// Called by children to notify their parent that they need an update.  	
		void 	requestUpdate (Node *child, bool forceParentUpdate=false)
		// Queue a 'needUpdate' call to a node safely.	
		static void 	queueNeedUpdate (Node *n)
		// Process queued 'needUpdate' calls.  	
		static void 	processQueuedUpdates (void)
		
	protected:
		Node* createChildImpl (void);
		Node* createChildImpl (const String &name);
		void setParent (Node *parent);
		// Internal method for setting whether the node is in the scene graph.
		void 	setInSceneGraph (bool inGraph);
		// Triggers the node to update it's combined transforms.  	
		void 	_updateFromParent (void) const;
		ObjectMap 	mObjectsByName;
		// Flag that determines if the bounding box of the node should be displayed.	
		bool 	mShowBoundingBox;
		// SceneManager which created this node. 	
		SceneManager * 	mCreator;
		// World-Axis aligned bounding box, updated only through _update. 	
		AxisAlignedBox 	mWorldAABB;
		// Whether to yaw around a fixed axis. 	
		bool 	mYawFixed;
		// Fixed axis to yaw around. 	
		ColumnVector 	mYawFixedAxis;
		// Auto tracking target. 	
		SceneNode * 	mAutoTrackTarget;
		// Tracking offset for fine tuning. 	
		ColumnVector 	mAutoTrackOffset;
		// Local 'normal' direction vector. 	
		ColumnVector 	mAutoTrackLocalDirection;
		// Is this node a current part of the scene graph? 	
		bool 	mIsInSceneGraph;
		// Pointer to parent node. 	
		Node * 	mParent;
		// Collection of pointers to direct children; hashmap for efficiency. 	
		ChildNodeMap 	mChildren;
		// List of children which need updating, used if self is not out of date but children are. 	
		ChildUpdateSet 	mChildrenToUpdate;
		// Flag to indicate own transform from parent is out of date. 	
		bool 	mNeedParentUpdate;
		// Flag indicating that all children need to be updated. 	
		bool 	mNeedChildUpdate;
		// Flag indicating that parent has been notified about update request. 	
		bool 	mParentNotified;
		// Flag indicating that the node has been queued for update. 	
		bool 	mQueuedForUpdate;
		// Friendly name of this node, can be automatically generated if you don't care. 	
		String 	mName;
		// Stores the orientation of the node relative to it's parent. 	
		Quaternion 	mOrientation;
		// Stores the position/translation of the node relative to its parent. 	
		ColumnVector 	mPosition;
		// Stores the scaling factor applied to this node. 	
		ColumnVector 	mScale;
		// Stores whether this node inherits orientation from it's parent. 	
		bool 	mInheritOrientation;
		// Stores whether this node inherits scale from it's parent. 	
		bool 	mInheritScale;
		// Material pointer should this node be rendered. 	
		MaterialPtr 	mpMaterial;
		// Cached combined orientation. 	
		ColumnVector 	derivedOrientation_;
		// Cached combined position. 	
		ColumnVector 	derivedPosition_;
		// Cached combined scale. 	
		ColumnVector 	derivedScale_;
		// Cached derived transform as a 4x4 matrix. 	
		SquareMatrix	cachedTransform_;
		bool	cachedTransformOutOfDate_;
				
		CustomParameterMap 	mCustomParameters;
		bool 	mPolygonModeOverrideable;
		bool 	mUseIdentityProjection;
		bool 	mUseIdentityView;

	protected:
		// Incremented count for next name extension.
		static unsigned long 	msNextGeneratedNameExt
		static QueuedUpdates 	msQueuedUpdates
		static const PlaneList 	msDummyPlaneList
		
	private:
		std::list<SceneNode*> childNodes_;
	};


};

#endif