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
 * Subject.cpp
 *
 *  Created on: Jul 31, 2008
 *      Author: yamokosk
 */

#include "Subject.h"

namespace sml
{

Subject::Subject() {}

Subject::~Subject() {}

void Subject::subscribe( Observer* obs )
{
	observers_.push_back(obs);
}

void Subject::unsubscribe( Observer* obs )
{
	ObserversIterator it = observers_.begin();
	for (; it != observers_.end(); ++it)
	{
		if ( (*it) == obs )
		{
			observers_.erase(it);
			return;
		}
	}
}

void Subject::notify(int hint)
{
	ObserversIterator it = observers_.begin();
	for (; it != observers_.end(); ++it)
	{
		(*it)->update(this, hint);
	}
}

} // Namespace: sml
