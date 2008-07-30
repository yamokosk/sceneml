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
	destroyAllEntities();

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

//---------------------------------------------------------------------
EntityCollection* SceneManager::getEntitiesByType(const std::string& typeName)
{
	EntityTypesMap::iterator i = entityTypes_.find(typeName);
	if ( i == entityTypesCollection_.end() )
	{
		// create
		EntityCollection* newCollection = new EntityCollection();
		entityTypesCollection_[typeName] = newCollection;
		return newCollection;
	}
	else
	{
		return i->second;
	}
}

Entity* SceneManager::createEntity(const std::string& name, const std::string& typeName, const PropertyCollection* params)
{
	EntityFactory* factory = Root::getSingleton().getEntityFactory(typeName);

	// Check for duplicate names
	EntityCollection* collection = getEntitiesByType(typeName);

	if (collection->entities_.find(name) != collection->entities_.end())
	{
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM,"An object of type '" + typeName + "' with name '" + name + "' already exists.");
	}

	Entity* newObj = factory->createInstance(name, this, params);
	collection->entities_[name] = newObj;
	return newObj;
}

void SceneManager::destroyEntity(const std::string& name, const std::string& typeName)
{
	EntityCollection* collection = getEntitiesByType(typeName);
	EntityFactory* factory = Root::getSingleton().getEntityFactory(typeName);

	EntityMap::iterator mi = collection->entities_.find(name);
	if (mi != collection->entities_.end())
	{
		factory->destroyInstance(mi->second);
		collection->entities_.erase(mi);
	}
}

void SceneManager::destroyAllEntitiesByType(const std::string& typeName)
{
	EntityCollection* collection = getEntitiesByType(typeName);
	EntityFactory* factory = Root::getSingleton().getEntityFactory(typeName);

	EntityMap::iterator i = collection->entities_.begin();
	for (; i != collection->entities_.end(); ++i)
	{
		// Only destroy our own
		if (i->second->_getManager() == this)
		{
			factory->destroyInstance(i->second);
		}
	}
	collection->entities_.clear();
}

void SceneManager::destroyAllEntities(void)
{
	EntityCollectionMap::iterator ci = entityTypes_.begin();

	for(;ci != entityTypes_.end(); ++ci)
	{
		EntityCollection* coll = ci->second;

		if (Root::getSingleton().hasEntityFactory(ci->first))
		{
			// Only destroy if we have a factory instance; otherwise must be injected
			EntityFactory* factory = Root::getSingleton().getEntityFactory(ci->first);
			EntityMap::iterator i = coll->entities_.begin();
			for (; i != coll->entities_.end(); ++i)
			{
				if (i->second->_getManager() == this)
				{
					factory->destroyInstance(i->second);
				}
			}
		}
		coll->entities_.clear();
	}

}

Entity* SceneManager::getEntity(const std::string& name, const std::string& typeName)
{
	EntityCollection* collection = getEntitiesByType(typeName);

	EntityMap::const_iterator mi = collection->entities_.find(name);
	if (mi == collection->entities_.end())
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,"Object named '" + name + "' does not exist.");
	}
	return mi->second;
}

bool SceneManager::hasEntity(const std::string& name, const std::string& typeName) const
{
	EntityCollectionMap::const_iterator i = entityTypes_.find(typeName);
	if (i == entityTypes_.end())
		return false;

	return (i->second->entities_.find(name) != i->second->entities_.end());
}

void SceneManager::destroyEntity(Entity* m)
{
	destroyEntity( m->getName(), m->getType() );
}

EntityIterator SceneManager::getEntityIterator(const std::string& typeName)
{
	EntityCollection* collection = getEntitiesByType(typeName);
	// Iterator not thread safe! Warned in header.
	return EntityIterator(collection->entities_.begin(), collection->entities_.end());
}

//---------------------------------------------------------------------
/*
Space* SceneManager::createSpace(const std::string& name, const std::string typeName, const PropertyCollection* params)
{

}

void SceneManager::destroySpace(const std::string& name, const std::string& typeName)
{

}

void SceneManager::destroySpace(Space* m)
{

}

void SceneManager::destroyAllSpacesByType(const std::string& typeName)
{

}

void SceneManager::destroyAllSpaces(void)
{

}

Space* SceneManager::getSpace(const std::string& name, const std::string& typeName)
{

}

bool SceneManager::hasSpace(const std::string& name, const std::string& typeName) const
{

}*/

//virtual SpaceIterator getSpaceIterator(const std::string& typeName)
//{
//
//}

void SceneManager::createEntityPair(Entity* e1, Entity* e2)
{
	// Make sure both named Entities already exist
	if ( !hasEntity(e1) )
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,"Entity named '" + e1->getName() + "' does not exist.");
	}

	if ( !hasEntity(e1) )
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,"Entity named '" + e2->getName() + "' does not exist.");
	}

	entityPairs_.push_back( EntityPair(e1,e2) );
}

EntityPairListIterator SceneManager::getEntityPairsIterator()
{
	return EntityPairListIterator(entityPairs_.being(), entityPairs_.end());
}

SceneQuery* performQuery(const std::string& typeName)
{
	SceneQuery* query =	Root::getSingleton().createSceneQuery(typeName);
	query->execute(this);
	return query;
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

} // namespace sml
