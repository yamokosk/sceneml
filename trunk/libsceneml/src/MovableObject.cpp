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
 * MovableObject.cpp
 *
 *  Created on: Jul 23, 2008
 *      Author: yamokosk
 */

#include <tinysg/MovableObject.h>
#include <tinysg/Node.h>

namespace TinySG
{

MovableObject::MovableObject() :
	Object(),
	visible_(true),
	parentNode_(NULL)
{
}

MovableObject::~MovableObject()
{
	if ( isAttached() )
		parentNode_->detachObject(this);
}

void MovableObject::notifyAttached(Node* parent)
{
	assert(!parentNode_ || !parent);
	bool different = (parent != parentNode_);
	parentNode_ = parent;
}

bool MovableObject::isAttached(void) const
{
	return (parentNode_ != NULL);
}

} // Namespace TinySG
