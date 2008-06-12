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
 
#ifndef _SCENEML_NODE_H_FILE_
#define _SCENEML_NODE_H_FILE_
 
namespace sceneml {

class SceneNode
{
public:
	SceneNode();
	virtual ~SceneNode();
	
	SceneNode* getParent();
	SceneNode* createChildSceneNode();
	void attachObject(Entity* obj);
	
	virtual const Quaternion & 	getOrientation () const
	// Returns a quaternion representing the nodes orientation.
	virtual void 	setOrientation (const Quaternion &q)
	// Sets the orientation of this node via a quaternion.
	virtual void 	setOrientation (Real w, Real x, Real y, Real z)
	// Sets the orientation of this node via quaternion parameters.
	virtual void 	resetOrientation (void)
	// Resets the nodes orientation (local axes as world axes, no rotation).
	virtual void 	setPosition (const Vector3 &pos)
	// Sets the position of the node relative to it's parent.
	virtual void 	setPosition (Real x, Real y, Real z)
	// Sets the position of the node relative to it's parent.
	virtual const Vector3 & 	getPosition (void) const
	// Gets the position of the node relative to it's parent. 
	
private:
	std::list<SceneNode*> childNodes_;
};
 
 
};
 
 #endif