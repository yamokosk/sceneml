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

#include <Singelton.h>

namespace sml
{

class Root : public Singleton<Root>
{
public:
	Root();
	virtual ~Root();

	// Register a new MovableObjectFactory which will create new MovableObject instances of a particular type, as identified by the getType() method.
	void 	addMovableObjectFactory (MovableObjectFactory *fact, bool overrideExisting=false);
	// Removes a previously registered MovableObjectFactory.
	void 	removeMovableObjectFactory (MovableObjectFactory *fact);
	// Checks whether a factory is registered for a given MovableObject type.
	bool 	hasMovableObjectFactory (const String &typeName) const;
	// Get a MovableObjectFactory for the given type.
	MovableObjectFactory * 	getMovableObjectFactory (const String &typeName);
	// Allocate the next MovableObject type flag.
	uint32 	_allocateNextMovableObjectTypeFlag (void);
	// Return an iterator over all the MovableObjectFactory instances currently registered.
	MovableObjectFactoryIterator 	getMovableObjectFactoryIterator (void) const;

protected:
	// List of plugin DLLs loaded.
	PluginLibList 	mPluginLibs;
	// List of Plugin instances registered.
	PluginInstanceList 	mPlugins;

	MovableObjectFactoryMap 	mMovableObjectFactoryMap
	MovableObjectFactory * 	mEntityFactory
};

}

#endif /* ROOT_H_ */
