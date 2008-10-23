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

#include "SceneGraph.h"

// Internal includes
#include "Node.h"

namespace TinySG
{

unsigned long SceneGraph::nextGeneratedNameExt_ = 1;

SceneGraph::SceneGraph()
{
	registerFactory( new NodeFactory() );

	rootNode_ = static_cast<Node*>(createObject("_WORLD_", Node::ObjectTypeID));
}

SceneGraph::~SceneGraph()
{
	clearScene();

	if (rootNode_) delete rootNode_;
}

void SceneGraph::clearScene(void)
{
	// Clear root node of all children
	rootNode_->removeAllChildren();

	// Delete all SceneNodes, except root that is
	destroyAllObjects( Node::ObjectTypeID );
}

void SceneGraph::update()
{
	// Cascade down the graph updating transforms & world bounds
	// In this implementation, just update from the root
	// Smarter SceneGraph subclasses may choose to update only
	//   certain scene graph branches
	rootNode_->update();
}

} // namespace TinySG
