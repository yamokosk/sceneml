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
/*
 * Root.cpp
 *
 *  Created on: Jul 23, 2008
 *      Author: yamokosk
 */

#include "Root.h"

#include <SceneManager.h>
#include <Entity.h>

namespace sml
{

template<> Root* Singleton<Root>::ms_Singleton = 0;
Root* Root::getSingletonPtr(void)
{
	return ms_Singleton;
}
Root& Root::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

Root::Root() :
	mEntityFactory(NULL),
	sceneMgr_(NULL)
{

}

Root::~Root()
{
	// Unload plugins
	PluginInstanceMap::iterator itr = registeredPlugins_.begin();
	for (; itr != registeredPlugins_.end(); ++itr)
	{
		(itr->second)->shutdown();
	}

	if (sceneMgr_) delete sceneMgr_;
	if (mEntityFactory) delete mEntityFactory;
}

SceneManager* Root::createSceneManager()
{
	if (!sceneMgr_) sceneMgr_ = new SceneManager();
	else
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM, "A scene manager already exists.");

	return sceneMgr_;
}

SceneManager* Root::getSceneManager()
{
	return sceneMgr_;
}

//---------------------------------------------------------------------
void Root::addEntityFactory(EntityFactory* fact)
{
	EntityFactoryMap::iterator facti = sceneObjectFactoryMap_.find(fact->getType());
	/*if (!overrideExisting && facti != sceneObjectFactoryMap_.end())
	{
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM,"A factory of type '" + fact->getType() + "' already exists.");
	}*/

	/*if (fact->requestTypeFlags())
	{
		if (facti != sceneObjectFactoryMap_.end() && facti->second->requestTypeFlags())
		{
			// Copy type flags from the factory we're replacing
			fact->_notifyTypeFlags(facti->second->getTypeFlags());
		}
		else
		{
			// Allocate new
			fact->_notifyTypeFlags(_allocateNextEntityTypeFlag());
		}
	}*/

	// Save
	sceneObjectFactoryMap_[fact->getType()] = fact;
}
//---------------------------------------------------------------------
bool Root::hasEntityFactory(const std::string& typeName) const
{
	return !(sceneObjectFactoryMap_.find(typeName) == sceneObjectFactoryMap_.end());
}
//---------------------------------------------------------------------
EntityFactory* Root::getEntityFactory(const std::string& typeName)
{
	EntityFactoryMap::iterator i = sceneObjectFactoryMap_.find(typeName);
	if (i == sceneObjectFactoryMap_.end())
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,"EntityFactory of type " + typeName + " does not exist");
	}
	return i->second;
}
//---------------------------------------------------------------------
/*uint32 Root::_allocateNextEntityTypeFlag(void)
{
	if (mNextEntityTypeFlag == SceneManager::USER_TYPE_MASK_LIMIT)
	{
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM, "Cannot allocate a type flag since all the available flags have been used.");

	}
	uint32 ret = mNextEntityTypeFlag;
	mNextEntityTypeFlag <<= 1;
	return ret;

}*/
//---------------------------------------------------------------------
void Root::removeEntityFactory(EntityFactory* fact)
{
	EntityFactoryMap::iterator i = sceneObjectFactoryMap_.find(
		fact->getType());
	if (i != sceneObjectFactoryMap_.end())
	{
		sceneObjectFactoryMap_.erase(i);
	}

}
//---------------------------------------------------------------------
/*Root::EntityFactoryIterator Root::getEntityFactoryIterator(void) const
{
	return EntityFactoryIterator(sceneObjectFactoryMap_.begin(),
		sceneObjectFactoryMap_.end());
}*/

void Root::addSceneQueryFactory (SceneQuery* query)
{
	return;
}

void Root::removeSceneQueryFactory (SceneQuery* query)
{
	return;
}

bool Root::hasSceneQueryFactory ( const std::string &typeName ) const
{
	return false;
}

/*SceneQuery* Root::getSceneQueryFactory (const std::string& typeName )
{
	return NULL;
}*/

void Root::registerPlugin( Plugin* p )
{
	std::string type = p->getType();
	// Check that we don't already have one of this type registered
	if ( registeredPlugins_.find( type ) != registeredPlugins_.end() )
	{
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM,"Plugin type " + type + " already registered");
	}

	p->initialize();
	p->registerFactories(this);
	p->registerQueries(this);

	registeredPlugins_[type] = p;
}

}
