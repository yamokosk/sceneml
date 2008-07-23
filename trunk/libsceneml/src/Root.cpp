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

Root::Root()
{
	// TODO Auto-generated constructor stub

}

Root::~Root()
{
	// TODO Auto-generated destructor stub
}

//---------------------------------------------------------------------
void Root::addSceneObjectFactory(SceneObjectFactory* fact, bool overrideExisting)
{
	SceneObjectFactoryMap::iterator facti = mSceneObjectFactoryMap.find(fact->getType());
	if (!overrideExisting && facti != mSceneObjectFactoryMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
			"A factory of type '" + fact->getType() + "' already exists.",
			"Root::addSceneObjectFactory");
	}

	if (fact->requestTypeFlags())
	{
		if (facti != mSceneObjectFactoryMap.end() && facti->second->requestTypeFlags())
		{
			// Copy type flags from the factory we're replacing
			fact->_notifyTypeFlags(facti->second->getTypeFlags());
		}
		else
		{
			// Allocate new
			fact->_notifyTypeFlags(_allocateNextSceneObjectTypeFlag());
		}
	}

	// Save
	mSceneObjectFactoryMap[fact->getType()] = fact;
}
//---------------------------------------------------------------------
bool Root::hasSceneObjectFactory(const String& typeName) const
{
	return !(mSceneObjectFactoryMap.find(typeName) == mSceneObjectFactoryMap.end());
}
//---------------------------------------------------------------------
SceneObjectFactory* Root::getSceneObjectFactory(const String& typeName)
{
	SceneObjectFactoryMap::iterator i = mSceneObjectFactoryMap.find(typeName);
	if (i == mSceneObjectFactoryMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
			"SceneObjectFactory of type " + typeName + " does not exist",
			"Root::getSceneObjectFactory");
	}
	return i->second;
}
//---------------------------------------------------------------------
uint32 Root::_allocateNextSceneObjectTypeFlag(void)
{
	if (mNextSceneObjectTypeFlag == SceneManager::USER_TYPE_MASK_LIMIT)
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
			"Cannot allocate a type flag since "
			"all the available flags have been used.",
			"Root::_allocateNextSceneObjectTypeFlag");

	}
	uint32 ret = mNextSceneObjectTypeFlag;
	mNextSceneObjectTypeFlag <<= 1;
	return ret;

}
//---------------------------------------------------------------------
void Root::removeSceneObjectFactory(SceneObjectFactory* fact)
{
	SceneObjectFactoryMap::iterator i = mSceneObjectFactoryMap.find(
		fact->getType());
	if (i != mSceneObjectFactoryMap.end())
	{
		mSceneObjectFactoryMap.erase(i);
	}

}
//---------------------------------------------------------------------
Root::SceneObjectFactoryIterator
Root::getSceneObjectFactoryIterator(void) const
{
	return SceneObjectFactoryIterator(mSceneObjectFactoryMap.begin(),
		mSceneObjectFactoryMap.end());

}


}
