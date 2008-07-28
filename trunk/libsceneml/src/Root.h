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

#include <map>

#include <Singleton.h>
#include <Exception.h>

namespace sml
{

class SceneMgr;
class SceneObjectFactory;

class Root : public Singleton<Root>
{
public:
	typedef std::map<std::string, SceneObjectFactory*> SceneObjectFactoryMap;

	Root();
	virtual ~Root();

	// Override standard Singleton retrieval.
	static Root& getSingleton(void);
	static Root* getSingletonPtr(void);

	SceneMgr* createSceneManager();
	SceneMgr* getSceneManager();

	// Register a new SceneObjectFactory which will create new SceneObject instances of a particular type, as identified by the getType() method.
	void addSceneObjectFactory (SceneObjectFactory *fact);
	// Removes a previously registered SceneObjectFactory.
	void removeSceneObjectFactory (SceneObjectFactory *fact);
	// Checks whether a factory is registered for a given SceneObject type.
	bool hasSceneObjectFactory (const std::string &typeName) const;
	// Get a SceneObjectFactory for the given type.
	SceneObjectFactory* getSceneObjectFactory (const std::string &typeName);
	// Allocate the next SceneObject type flag.
	//uint32 	_allocateNextSceneObjectTypeFlag (void);
	// Return an iterator over all the SceneObjectFactory instances currently registered.
	//SceneObjectFactoryIterator 	getSceneObjectFactoryIterator (void) const;

protected:
	// List of plugin DLLs loaded.
	//PluginLibList 	mPluginLibs;
	// List of Plugin instances registered.
	//PluginInstanceList 	mPlugins;

	SceneObjectFactoryMap 	sceneObjectFactoryMap_;
	SceneObjectFactory* 	mEntityFactory;
	SceneMgr*				sceneMgr_;
};

}

#endif /* ROOT_H_ */
