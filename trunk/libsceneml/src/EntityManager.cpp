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

namespace TinySG
{

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

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

} // End namespace TinySG
