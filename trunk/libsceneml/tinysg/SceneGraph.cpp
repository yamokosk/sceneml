/*************************************************************************
 * TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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

#include "Exception.h"
#include "Query.h"

#include <boost/foreach.hpp>


namespace TinySG
{

const std::string SceneGraph::NodeType("node");
const std::string SceneGraph::World("_WORLD_");
unsigned long SceneGraph::NextGeneratedNameExt(0);


SceneGraph::SceneGraph() :
	rootNode_(NULL),
	nodeFactory_( NodeType )
{
	registerFactory( &nodeFactory_ );
	rootNode_ = createNode(SceneGraph::World);
}

SceneGraph::~SceneGraph()
{
	clearScene();

	if (rootNode_ != NULL) delete rootNode_;
}

SceneNode* SceneGraph::createNode(const PropertyCollection* params)
{
	std::stringstream ss; ss << "unnamed_" << NextGeneratedNameExt++;
	return createNode(ss.str(), params);
}

SceneNode* SceneGraph::createNode(const std::string& name, const PropertyCollection* params)
{
	return createAndCastObject<SceneNode>(name, NodeType, params);
}

void SceneGraph::destroyNode(const std::string& name)
{
	destroyObject(name, NodeType);
}

void SceneGraph::destroyNode(SceneNode* node)
{
	destroyObject(node);
}

void SceneGraph::destroyAllNodes()
{
	destroyAllObjects( NodeType );
}

SceneNode* SceneGraph::getNode(const std::string& name) const
{
	return getAndCastObject<SceneNode>(name, NodeType);
}

void SceneGraph::clearScene(void)
{
	// Clear root node of all children
	rootNode_->removeAllChildren();

	// Delete all SceneNodes, except root that is
	destroyAllNodes();
}

void SceneGraph::update()
{
	// Cascade down the graph updating transforms & world bounds
	// In this implementation, just update from the root
	// Smarter SceneGraph subclasses may choose to update only
	//   certain scene graph branches
	rootNode_->update();
}

void SceneGraph::save(TinySG::Archive& ar)
{
	ObjectManager::ObjectCollection* oc = getCollection(NodeType);

	ar.createCollection("SceneGraph", oc->objects.size() );
	std::pair<std::string, Object*> pair;
	BOOST_FOREACH( pair, oc->objects )
	{
		SceneNode* node = boost::polymorphic_downcast<SceneNode*>(pair.second);
		ar.serializeObject("SceneGraph", *node );
	}
}

void SceneGraph::load(TinySG::Archive& ar)
{
	//Archive::Collection* c = ar.getCollection( "SceneGraph" );
	Archive::Collection* c = ar.getFirstCollection();

	if ( c != NULL )
	{
		for (unsigned int n=0; n < c->size(); ++n)
		{
			PropertyCollection pc = c->objects[n];
			createNode(pc.getValue("name"), &pc);
		}
	}
}

} // namespace TinySG
