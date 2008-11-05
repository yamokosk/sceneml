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
 * ObjectManager.h
 *
 *  Created on: Sep 18, 2008
 *      Author: yamokosk
 */

#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

// External
#include <string>

// Internal
#include <tinysg/Exception.h>
#include <tinysg/Object.h>
#include <tinysg/Query.h>
#include <tinysg/Map.h>

namespace TinySG
{

class ObjectManager
{
public:
	typedef MAP<std::string, Object*> Objects;
	typedef Objects::iterator ObjectsIterator;
	typedef Objects::const_iterator ObjectsConstIterator;

	struct ObjectCollection
	{
		std::string objectType;
		ObjectFactory* factory;
		Objects objects;
	};

	typedef MAP<std::string, ObjectCollection*> Collections;
	typedef Collections::iterator CollectionsIterator;
	typedef Collections::const_iterator CollectionsConstIterator;

	typedef MAP<std::string, Query*> Queries;
	typedef Queries::iterator QueryIterator;

public:
	ObjectManager();
	virtual ~ObjectManager();

	// Object management
	virtual Object* createObject(const std::string& name, const std::string& type, const PropertyCollection* params=NULL);
	virtual void destroyObject(const std::string& name, const std::string& type);
	virtual void destroyObject(Object* obj);
	virtual void destroyAllObjects( const std::string& type );
	virtual Object* getObject(const std::string& name, const std::string& type) const;

	// Factory management
	virtual void registerFactory( ObjectFactory* obj );
	virtual ObjectFactory* getFactory( const std::string& type ) const;

	// Collections management
	void destroyCollection( const std::string& type );
	void destroyCollection( ObjectCollection* collection );
	void destroyAllCollections();
	ObjectCollection* getCollection( const std::string& type ) const;

	// Query management
	virtual QueryResult* performQuery( const std::string queryName, const PropertyCollection* params=0 );
	virtual void addQuery( Query* query );

private:
	Collections objectCollections_;
	Queries queries_;
};

}

#endif /* OBJECTMANAGER_H_ */
