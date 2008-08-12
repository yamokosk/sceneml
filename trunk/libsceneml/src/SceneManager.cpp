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

#include "SceneManager.h"
#include "Node.h"

namespace TinySG
{

SceneManager::SceneManager()
{
	rootNode_ = new Node(this, "WORLD");
}

SceneManager::~SceneManager()
{
	clearScene();

	if (rootNode_) delete rootNode_;
}

Node* SceneManager::createNode()
{
    Node* n = new Node(this);
    assert(nodes_.find(n->getName()) == nodes_.end());
    nodes_[n->getName()] = n;
    return n;
}

Node* SceneManager::createNode(const std::string& name)
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

void SceneManager::destroyNode(const std::string& name)
{
	NodeIterator i = nodes_.find(name);

	if (i == nodes_.end())
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Node '" + name + "' not found.");
	}

	// detach from parent (don't do this in destructor since bulk destruction
	// behaves differently)
	Node* parentNode = i->second->getParent();
	if (parentNode)
	{
		parentNode->removeChild(i->second);
	}
	delete i->second;
	nodes_.erase(i);
}

Node* SceneManager::getRootNode() const
{
	return rootNode_;
}

Node* SceneManager::getNode(const std::string& name) const
{
	NodeConstIterator i = nodes_.find(name);

    if (i == nodes_.end())
    {
        SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "SceneNode '" + name + "' not found.");
    }

    return i->second;
}

bool SceneManager::hasSceneNode(const std::string& name) const
{
	return (nodes_.find(name) != nodes_.end());
}

void SceneManager::clearScene(void)
{
	//destroyAllStaticGeometry();

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

QueryResult* SceneManager::getQueryResult(const std::string& typeName)
{
	ResultMapIterator itr = results_.find(typeName);

	if ( itr == results_.end() )
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,"Result type '" + typeName + "' does not exist.");
	}

	return itr->second;
}

void SceneManager::destroyAllQueryResults()
{
	ResultMapIterator itr = results_.begin();
	for (; itr != results_.end(); ++itr)
	{
		(itr->second)->getCreator()->deleteResult( itr->second );
	}
	results_.clear();
}

void SceneManager::_addResult(QueryResult* result)
{
	std::string typeName = result->getType();
	if ( results_.find( typeName ) != results_.end() )
	{
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM,"A result of type '" + typeName + "' already exists.");
	}

	results_[ typeName ] = result;
}

void SceneManager::update()
{
	_updateSceneGraph();
	_performQueries();
}

void SceneManager::_addQuery(SceneQuery* query)
{
	// Lock query
	queries_.push(query);
	// Unlock query
}

void SceneManager::_performQueries()
{
	// Lock the mutex

	// Clean out current query result list
	destroyAllQueryResults();

	while ( !queries_.empty() )
	{
		queries_.front()->execute( this );
		queries_.pop();
	}

	// Unlock the mutex
}

void SceneManager::_updateSceneGraph()
{
	// Process queued needUpdate calls
	Node::processQueuedUpdates();

    // Cascade down the graph updating transforms & world bounds
    // In this implementation, just update from the root
    // Smarter SceneManager subclasses may choose to update only
    //   certain scene graph branches
	rootNode_->_update(true, false);
}

void SceneManager::update(int hint)
{
	/*SceneQuery* query = boost::polymorphic_downcast<SceneQuery*>(sub);

	switch (hint)
	{
	case QUERY_STARTED:
		break;
	case QUERY_COMPLETED:
		QueryResult result = query->getResult();
		_addResult(result);
		_addQuery(query);
		break;
	default:
		break;
	}*/
}

} // namespace TinySG
