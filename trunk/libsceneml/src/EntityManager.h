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

#ifndef _ENTITYMGR_H_FILE_
#define _ENTITYMGR_H_FILE_


namespace TinySG {

// Forward declarations
class Entity;

class EntityManager
{
public:
	// Entities
	typedef stdext::hash_map< std::string, Entity*> EntityMap;
	typedef EntityMap::iterator EntityIterator;
	// Collection of one type of Entity objects.. ODE, LINCANNY, etc.
	struct EntityCollection
	{
		EntityMap entities_;
	};
	typedef stdext::hash_map< std::string, EntityCollection*> EntityCollectionMap;

	// Entity pairs
	typedef std::pair<Entity*, Entity*> EntityPair;
	typedef std::list<EntityPair> EntityPairList;
	typedef EntityPairList::iterator EntityPairListIterator;

public:
	EntityManager();
	~EntityManager();

	// Entities
	Entity* createEntity(const std::string& name,const std::string& typeName, const PropertyCollection* params = 0);
	void destroyEntity(const std::string& name, const std::string& typeName);
	void destroyEntity(Entity* m);
	void destroyAllEntitiesByType(const std::string& typeName);
	void destroyAllEntities(void);
	Entity* getEntity(const std::string& name, const std::string& typeName);
	EntityCollection* getEntitiesByType(const std::string& typeName);
	bool hasEntity(const std::string& name, const std::string& typeName) const;
	bool hasEntity(const Entity* e) const;
	//EntityIterator getEntityIterator(const std::string& typeName);

	// Entity pairs
	void createEntityPair(Entity* e1, Entity* e2);
	//EntityPairListIterator getEntityPairsIterator();

private:
	EntityCollectionMap entityTypes_;
	EntityPairList entityPairs_;
};

}

#endif
