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

#include "SceneMgr.h"
#include "Node.h"

namespace sml 
{

SceneMgr::SceneMgr()
{
	rootNode_ = new Node(this, "WORLD");
}

SceneMgr::~SceneMgr()
{
	clearScene();

	if (rootNode_) delete rootNode_;
}

Node* SceneMgr::getRootNode() const
{
	return rootNode_;
}

Node* SceneMgr::createNode()
{
    Node* n = new Node(this);
    assert(nodes_.find(n->getName()) == nodes_.end());
    nodes_[n->getName()] = n;
    return n;
}

Node* SceneMgr::createNode(const std::string& name)
{
	// Check name not used
	if (nodes_.find(name) != nodes_.end())
	{
		std::ostringstream msg;
		msg << "A node with the name " << name << " already exists.";
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM, msg.str());
	}

	Node* n = new Node(this, name);
	nodes_[n->getName()] = n;
	return n;
}

void SceneMgr::clearScene(void)
{
	//destroyAllStaticGeometry();
	//destroyAllMovableObjects();

	// Clear root node of all children
	rootNode_->removeAllChildren();
	//rootNode_->detachAllObjects();

	// Delete all SceneNodes, except root that is
	for (NodeIterator i = nodes_.begin(); i != nodes_.end(); ++i)
	{
		delete i->second;
	}
	nodes_.clear();
	//mAutoTrackingSceneNodes.clear();

	// Clear animations
    //destroyAllAnimations();

    // Remove sky nodes since they've been deleted
    //mSkyBoxNode = mSkyPlaneNode = mSkyDomeNode = 0;
    //mSkyBoxEnabled = mSkyPlaneEnabled = mSkyDomeEnabled = false;

	// Clear render queue, empty completely
	//if (mRenderQueue)
	//	mRenderQueue->clear(true);

}

} // namespace sml
