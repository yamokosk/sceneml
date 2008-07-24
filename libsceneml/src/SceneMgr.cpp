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

void SceneMgr::destroyNode(const std::string& name)
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

Node* SceneMgr::getRootNode() const
{
	return rootNode_;
}

Node* SceneMgr::getNode(const std::string& name) const
{
	NodeConstIterator i = nodes_.find(name);

    if (i == nodes_.end())
    {
        SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "SceneNode '" + name + "' not found.");
    }

    return i->second;
}

bool SceneMgr::hasSceneNode(const std::string& name) const
{
	return (nodes_.find(name) != nodes_.end());
}

void SceneMgr::clearScene(void)
{
	//destroyAllStaticGeometry();
	//destroyAllSceneObjects();

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

void SceneMgr::_updateSceneGraph()
{
	// Process queued needUpdate calls
	Node::processQueuedUpdates();

    // Cascade down the graph updating transforms & world bounds
    // In this implementation, just update from the root
    // Smarter SceneMgr subclasses may choose to update only
    //   certain scene graph branches
	rootNode_->_update(true, false);
}

//SceneObjectCollection* SceneMgr::getSceneObjectCollection(const std::string& typeName)
SceneObjectCollection* SceneMgr::getSceneObjectCollection(const std::string& typeName)
{
	// lock collection mutex
	//OGRE_LOCK_MUTEX(mSceneObjectCollectionMapMutex)

	SceneObjectCollectionMap::iterator i = sceneObjectCollectionMap_.find(typeName);
	if (i == sceneObjectCollectionMap_.end())
	{
		// create
		SceneObjectCollection* newCollection = new SceneObjectCollection();
		sceneObjectCollectionMap_[typeName] = newCollection;
		return newCollection;
	}
	else
	{
		return i->second;
	}
}

SceneObject* SceneMgr::createSceneObject(const std::string& name, const std::string& typeName, const PropertyCollection* params)
{
	SceneObjectFactory* factory =
		Root::getSingleton().getSceneObjectFactory(typeName);
	// Check for duplicate names
	SceneObjectCollection* objectMap = getSceneObjectCollection(typeName);

	{
		//OGRE_LOCK_MUTEX(objectMap->mutex)

		if (objectMap->map.find(name) != objectMap->map.end())
		{
			SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM,"An object of type '" + typeName + "' with name '" + name + "' already exists.");
		}

		SceneObject* newObj = factory->createInstance(name, this, params);
		objectMap->map[name] = newObj;
		return newObj;
	}

}
//---------------------------------------------------------------------
void SceneMgr::destroySceneObject(const std::string& name, const std::string& typeName)
{
	SceneObjectCollection* objectMap = getSceneObjectCollection(typeName);
	SceneObjectFactory* factory = Root::getSingleton().getSceneObjectFactory(typeName);

	{
		//OGRE_LOCK_MUTEX(objectMap->mutex)

		SceneObjectMap::iterator mi = objectMap->map.find(name);
		if (mi != objectMap->map.end())
		{
			factory->destroyInstance(mi->second);
			objectMap->map.erase(mi);
		}
	}
}
//---------------------------------------------------------------------
void SceneMgr::destroyAllSceneObjectsByType(const std::string& typeName)
{
	SceneObjectCollection* objectMap = getSceneObjectCollection(typeName);
	SceneObjectFactory* factory =
		Root::getSingleton().getSceneObjectFactory(typeName);

	{
		//OGRE_LOCK_MUTEX(objectMap->mutex)
		SceneObjectMap::iterator i = objectMap->map.begin();
		for (; i != objectMap->map.end(); ++i)
		{
			// Only destroy our own
			if (i->second->_getManager() == this)
			{
				factory->destroyInstance(i->second);
			}
		}
		objectMap->map.clear();
	}
}
//---------------------------------------------------------------------
void SceneMgr::destroyAllSceneObjects(void)
{
	// Lock collection mutex
	//OGRE_LOCK_MUTEX(mSceneObjectCollectionMapMutex)

	SceneObjectCollectionMap::iterator ci = sceneObjectCollectionMap_.begin();

	for(;ci != sceneObjectCollectionMap_.end(); ++ci)
	{
		SceneObjectCollection* coll = ci->second;

		// lock map mutex
		//OGRE_LOCK_MUTEX(coll->mutex)

		if (Root::getSingleton().hasSceneObjectFactory(ci->first))
		{
			// Only destroy if we have a factory instance; otherwise must be injected
			SceneObjectFactory* factory = Root::getSingleton().getSceneObjectFactory(ci->first);
			SceneObjectMap::iterator i = coll->map.begin();
			for (; i != coll->map.end(); ++i)
			{
				if (i->second->_getManager() == this)
				{
					factory->destroyInstance(i->second);
				}
			}
		}
		coll->map.clear();
	}

}
//---------------------------------------------------------------------
SceneObject* SceneMgr::getSceneObject(const std::string& name, const std::string& typeName)
{
	SceneObjectCollection* objectMap = getSceneObjectCollection(typeName);

	{
		//OGRE_LOCK_MUTEX(objectMap->mutex)
		SceneObjectMap::const_iterator mi = objectMap->map.find(name);
		if (mi == objectMap->map.end())
		{
			SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,"Object named '" + name + "' does not exist.");
		}
		return mi->second;
	}

}
//-----------------------------------------------------------------------
bool SceneMgr::hasSceneObject(const std::string& name, const std::string& typeName) const
{
	//OGRE_LOCK_MUTEX(mSceneObjectCollectionMapMutex)

	SceneObjectCollectionMap::const_iterator i = sceneObjectCollectionMap_.find(typeName);
	if (i == sceneObjectCollectionMap_.end())
		return false;

	{
		//OGRE_LOCK_MUTEX(i->second->mutex)
		return (i->second->map.find(name) != i->second->map.end());
	}
}

//---------------------------------------------------------------------
/*SceneMgr::SceneObjectIterator SceneMgr::getSceneObjectIterator(const std::string& typeName)
{
	SceneObjectCollection* objectMap = getSceneObjectCollection(typeName);
	// Iterator not thread safe! Warned in header.
	return SceneObjectIterator(objectMap->map.begin(), objectMap->map.end());
}*/
//---------------------------------------------------------------------
void SceneMgr::destroySceneObject(SceneObject* m)
{
	destroySceneObject(m->getName(), "DUMMY"); //m->getMovableType());
}
//---------------------------------------------------------------------
/*void SceneMgr::injectSceneObject(SceneObject* m)
{
	SceneObjectCollection* objectMap = getSceneObjectCollection(m->getMovableType());
	{
		//OGRE_LOCK_MUTEX(objectMap->mutex)

		objectMap->map[m->getName()] = m;
	}
}*/



} // namespace sml
