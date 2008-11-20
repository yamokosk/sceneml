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
 * Root.h
 *
 *  Created on: Jul 23, 2008
 *      Author: yamokosk
 */

#ifndef ROOT_H_
#define ROOT_H_

// Logging
#include <log4cxx/logger.h>

// Internal includes
#include <tinysg/Exception.h>
#include <tinysg/Singleton.h>
#include <tinysg/Query.h>
#include <tinysg/Plugin.h>
#include <tinysg/Map.h>
#include <tinysg/Archive.h>
#include <tinysg/Object.h>
#include <tinysg/ObjectManager.h>
#include <tinysg/SceneGraph.h>

#include <tinysg/dll.h>

namespace TinySG
{

class Root : public Singleton<Root>
{
	static log4cxx::LoggerPtr logger;

	typedef MAP<std::string, Plugin*> PluginInstanceMap;
public:
	Root();
	virtual ~Root();

	// Override standard Singleton retrieval.
	static Root& getSingleton(void);
	static Root* getSingletonPtr(void);

	void loadScene(const std::string& filename);
	void saveScene(const std::string& filename);

	// Managers
	SceneGraph*	getSceneGraph() const {return graph_;}
	ObjectManager*	getEntityManager() const {return objMgr_;}
	const SceneGraph* getSceneManager() const {return graph_;}
	const ObjectManager* getMeshManager() const {return objMgr_;}

	// Register a new EntityFactory which will create new Entity instances of a particular type, as identified by the getType() method.
	void addObjectFactory (ObjectFactory *fact);
	// Get a EntityFactory for the given type.
	ObjectFactory* getObjectFactory (const std::string &typeName);

	// Register new queries with their respective managers
	void addSceneQuery(Query* query);
	void addObjectQuery(Query* query);

	// Plugins
	// Internal methods
	void loadPlugin(const std::string& libName);
	// Container for all externally loaded libraries (aka plugins)
	std::vector<DynamicallyLoadedLibrary*> libraryHandles_;
	// Internal method for registering loaded plugins
	void registerPlugin( Plugin* );

private:
	ObjectManager* createObjectManager() {};
	SceneGraph* createSceneGraph() {};

	SceneGraph*	graph_;
	ObjectManager* objMgr_;

	PluginInstanceMap	registeredPlugins_;
};

}

#endif /* ROOT_H_ */
