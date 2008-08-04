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
 * Root.h
 *
 *  Created on: Jul 23, 2008
 *      Author: yamokosk
 */

#ifndef ROOT_H_
#define ROOT_H_

// Standard includes
#include <map>

// Internal includes
#include <Singleton.h>
#include <SceneQuery.h>
#include <Plugin.h>
#include <Exception.h>

namespace sml
{

class SceneManager;
class EntityFactory;

class Root : public Singleton<Root>
{
public:
	typedef std::map<std::string, EntityFactory*> EntityFactoryMap;
	typedef std::map<std::string, Plugin*> PluginInstanceMap;

	Root();
	virtual ~Root();

	// Override standard Singleton retrieval.
	static Root& getSingleton(void);
	static Root* getSingletonPtr(void);

	// SceneManager
	SceneManager* createSceneManager();
	SceneManager* getSceneManager();

	// Register a new EntityFactory which will create new Entity instances of a particular type, as identified by the getType() method.
	void addEntityFactory (EntityFactory *fact);
	// Removes a previously registered EntityFactory.
	void removeEntityFactory (EntityFactory *fact);
	// Checks whether a factory is registered for a given Entity type.
	bool hasEntityFactory (const std::string &typeName) const;
	// Get a EntityFactory for the given type.
	EntityFactory* getEntityFactory (const std::string &typeName);

	// Register a new SceneQuery
	void addSceneQueryFactory (SceneQuery* query);
	void removeSceneQueryFactory (SceneQuery* query);
	bool hasSceneQueryFactory ( const std::string &typeName ) const;
	SceneQuery* createSceneQuery (const std::string& typeName );

	// Plugins
	void registerPlugin( Plugin* );

protected:
	EntityFactoryMap 	sceneObjectFactoryMap_;
	EntityFactory* 	mEntityFactory;
	SceneManager*		sceneMgr_;
	PluginInstanceMap registeredPlugins_;
};

}

#endif /* ROOT_H_ */
