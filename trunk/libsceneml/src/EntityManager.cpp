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

#include "EntityManager.h"
#include "Entity.h"

namespace tinysg
{

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{
	destroyAllEntities();
}

EntityManager::EntityCollection* EntityManager::getEntitiesByType(const std::string& typeName)
{
	EntityCollectionMap::iterator i = entityTypes_.find(typeName);
	if ( i == entityTypes_.end() )
	{
		// create
		EntityManager::EntityCollection* newCollection = new EntityManager::EntityCollection();
		entityTypes_[typeName] = newCollection;
		return newCollection;
	}
	else
	{
		return i->second;
	}
}

Entity* EntityManager::createEntity(const std::string& name, const std::string& typeName, const PropertyCollection* params)
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

void EntityManager::destroyEntity(const std::string& name, const std::string& typeName)
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

void EntityManager::destroyAllEntitiesByType(const std::string& typeName)
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

void EntityManager::destroyAllEntities(void)
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

Entity* EntityManager::getEntity(const std::string& name, const std::string& typeName)
{
	EntityCollection* collection = getEntitiesByType(typeName);

	EntityMap::const_iterator mi = collection->entities_.find(name);
	if (mi == collection->entities_.end())
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,"Object named '" + name + "' does not exist.");
	}
	return mi->second;
}

bool EntityManager::hasEntity(const std::string& name, const std::string& typeName) const
{
	EntityCollectionMap::const_iterator i = entityTypes_.find(typeName);
	if (i == entityTypes_.end())
		return false;

	return (i->second->entities_.find(name) != i->second->entities_.end());
}

inline
bool EntityManager::hasEntity(const Entity* e) const
{
	return hasEntity(e->getName(), e->getType());
}

void EntityManager::destroyEntity(Entity* m)
{
	destroyEntity( m->getName(), m->getType() );
}

/*EntityManager::EntityIterator EntityManager::getEntityIterator(const std::string& typeName)
{
	EntityManager::EntityCollection* collection = getEntitiesByType(typeName);
	// Iterator not thread safe! Warned in header.
	return EntityManager::EntityIterator(collection->entities_.begin(), collection->entities_.end());
}*/

void EntityManager::createEntityPair(Entity* e1, Entity* e2)
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

/*EntityManager::EntityPairListIterator EntityManager::getEntityPairsIterator()
{
	return EntityManager::EntityPairListIterator(entityPairs_.being(), entityPairs_.end());
}*/

} // End namespace tinysg
