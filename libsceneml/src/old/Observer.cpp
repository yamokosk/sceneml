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
 * Observer.cpp
 *
 *  Created on: Aug 4, 2008
 *      Author: yamokosk
 */

#include "Observer.h"
#include "Subject.h"

namespace TinySG
{

Observer::Observer() :
	subject_(NULL)
{}

Observer::~Observer()
{
	if (subject_) subject_->unsubscribe(this);
}

void Observer::listen(Subject* sub)
{
	if (subject_)
	{
		subject_->unsubscribe(this);
	}
	subject_ = sub;
	subject_->subscribe(this);
}

} // Namespace: TinySG
