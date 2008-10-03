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
 * ObjectManager.cpp
 *
 *  Created on: Sep 18, 2008
 *      Author: yamokosk
 */

#include "ObjectManager.h"

namespace TinySG
{

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	destroyAllCollections();
}

Object* ObjectManager::createObject(const std::string& name, const std::string& type, const PropertyCollection* params)
{
	// First get the collection
	ObjectCollection* collection = getCollection(type);

	// Check name not used
	if ( collection->objects.find(name) != collection->objects.end() )
	{
		std::ostringstream msg;
		msg << "An object of type \"" << type << "\" with the name \"" << name << "\" already exists.";
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM, msg.str());
	}

	Object* obj = collection->factory->createInstance(name, this, params);
	collection->objects[obj->getName()] = obj;
	return obj;
}

void ObjectManager::destroyObject(const std::string& name, const std::string& type)
{
	ObjectCollection* collection = getCollection( type );

	// Find the object in question
	ObjectsIterator iter = collection->objects.find( name );
	if ( iter == collection->objects.end() )
	{
		std::ostringstream msg;
		msg << "The object of type \"" << type << "\" and \"" << name << "\" does not exist.";
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, msg.str());
	}

	// Factories control object creation and deletion
	collection->factory->destroyInstance( iter->second );

	// Now just remove this item from the collection
	collection->objects.erase( iter );
}

void ObjectManager::destroyObject( Object* obj )
{
	destroyObject( obj->getName(), obj->getType() );
}

void ObjectManager::destroyAllObjects( const std::string& type )
{
	ObjectCollection* collection = getCollection( type );

	// Iterate over all objects in the collection, destroying them one by one
	ObjectsIterator iter = collection->objects.begin();
	for (; iter != collection->objects.end(); ++iter)
	{
		destroyObject( iter->second );
	}
}

Object* ObjectManager::getObject(const std::string& name, const std::string& type) const
{
	ObjectCollection* collection = getCollection( type );

	ObjectsIterator iter = collection->objects.find( name );
	if ( iter == collection->objects.end() )
	{
		std::ostringstream msg;
		msg << "The object of type \"" << type << "\" and \"" << name << "\" does not exist.";
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, msg.str());
	}

	return iter->second;
}

void ObjectManager::registerFactory( ObjectFactory* fact )
{
	CollectionsIterator iter = objectCollections_.find( fact->getType() );

	if ( iter != objectCollections_.end() )
	{
		std::ostringstream msg;
		msg << "An object factory of type \"" << fact->getType() << "\" is already registered.";
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM, msg.str());
	}

	ObjectCollection* collection = new ObjectCollection();
	collection->objectType = fact->getType();
	collection->factory = fact;
	objectCollections_[ collection->objectType ] = collection;
}

ObjectFactory* ObjectManager::getFactory( const std::string& type ) const
{
	ObjectCollection* collection = getCollection(type);
	return collection->factory;
}

void ObjectManager::destroyCollection( const std::string& type )
{
	CollectionsIterator iter = objectCollections_.find(type);

	if ( iter == objectCollections_.end() )
	{
		std::ostringstream msg;
		msg << "The object collection of type \"" << type << "\" does not exist.";
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, msg.str());
	}

	// Delete all the objects stored in the collection
	destroyAllObjects( iter->second->objectType );

	// Delete the factory.. we control these too.
	delete iter->second->factory;

	// Finally erase this collection
	objectCollections_.erase( iter );
}

void ObjectManager::destroyCollection( ObjectCollection* collection )
{
	destroyCollection( collection->objectType );
}

void ObjectManager::destroyAllCollections()
{
	// Iterate over all objects in the collection, destroying them one by one
	CollectionsIterator iter = objectCollections_.begin();
	for (; iter != objectCollections_.end(); ++iter)
	{
		destroyCollection( iter->second );
	}
}

ObjectManager::ObjectCollection* ObjectManager::getCollection( const std::string& type ) const
{
	CollectionsConstIterator iter = objectCollections_.find(type);

	if ( iter == objectCollections_.end() )
	{
		std::ostringstream msg;
		msg << "The object collection of type \"" << type << "\" does not exist.";
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, msg.str());
	}

	return iter->second;
}

}
