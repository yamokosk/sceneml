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
 * Entity.cpp
 *
 *  Created on: Jul 23, 2008
 *      Author: yamokosk
 */

#include "Entity.h"
#include "Node.h"

namespace sml
{

Entity::Entity() :
        visible_(true),
        parentNode_(NULL),
        manager_(NULL),
        creator_(NULL)
{
}

Entity::Entity(const std::string& name) :
        visible_(true),
        parentNode_(NULL),
        manager_(NULL),
        name_(name),
        creator_(NULL)
{
}

Entity::~Entity()
{
        if (parentNode_)
                parentNode_->detachObject(this);
}

void Entity::_notifyAttached(Node* parent)
{
        assert(!parentNode_ || !parent);

        bool different = (parent != parentNode_);

        parentNode_ = parent;
}

bool Entity::isAttached(void) const
{
        return (parentNode_ != 0);
}


EntityFactory::EntityFactory()
{}

EntityFactory::~EntityFactory()
{}

Entity* EntityFactory::createInstance(const std::string& name, SceneManager* manager,
                const PropertyCollection* params)
{
        Entity* m = createInstanceImpl(name, params);
        m->_notifyCreator(this);
        m->_notifyManager(manager);
        return m;
}

} // Namespace sml
