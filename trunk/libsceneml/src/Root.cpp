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
/*
 * Root.cpp
 *
 *  Created on: Jul 23, 2008
 *      Author: yamokosk
 */

#include <tinysg/Root.h>

#include <tinysg/SceneGraph.h>
#include <tinysg/MovableObject.h>

namespace TinySG
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
	graph_(NULL),
	objMgr_(NULL)
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

	if (graph_) delete graph_;
	if (objMgr_) delete objMgr_;
}

void Root::load(const std::string& filename)
{
	Archive ar;
	ar.loadFromXML(filename);
	graph_->load(ar);
	objMgr_->load(ar);
}

void Root::save(const std::string& filename)
{
	Archive ar;
	graph_->save(ar);
	objMgr_->save(ar);
	ar.writeToXML(filename);
}

/*
SceneGraph* Root::createSceneManager()
{
	if (!graph_) graph_ = new SceneGraph();
	else
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM, "A scene graph already exists.");

	return graph_;
}

//---------------------------------------------------------------------
void Root::addObjectFactory(ObjectFactory* fact)
{
	ObjectFactoryMap::iterator facti = sceneObjectFactoryMap_.find(fact->getType());

	// Save
	sceneObjectFactoryMap_[fact->getType()] = fact;
}
//---------------------------------------------------------------------
bool Root::hasObjectFactory(const std::string& typeName) const
{
	return !(sceneObjectFactoryMap_.find(typeName) == sceneObjectFactoryMap_.end());
}
//---------------------------------------------------------------------
ObjectFactory* Root::getObjectFactory(const std::string& typeName)
{
	ObjectFactoryMap::iterator i = sceneObjectFactoryMap_.find(typeName);
	if (i == sceneObjectFactoryMap_.end())
	{
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,"ObjectFactory of type " + typeName + " does not exist");
	}
	return i->second;
}
//---------------------------------------------------------------------
void Root::removeObjectFactory(ObjectFactory* fact)
{
	ObjectFactoryMap::iterator i = sceneObjectFactoryMap_.find(
		fact->getType());
	if (i != sceneObjectFactoryMap_.end())
	{
		sceneObjectFactoryMap_.erase(i);
	}
}*/

void Root::addQuery (Query* query)
{
	graph_->addQuery(query);
}

void Root::addObjectQuery(Query* query)
{
	objMgr_->addQuery(query);
}

/*Query* Root::getQueryFactory (const std::string& typeName )
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
