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

#ifndef _TINYSG_SCENEMGR_H_FILE_
#define _TINYSG_SCENEMGR_H_FILE_

// External includes
#include <string>
#include <utility>
#include <queue>

// Internal includes
#include "Exception.h"
#include "ObjectManager.h"
#include "Map.h"

namespace TinySG {

// Forward declarations
class Node;

class SceneGraph : public ObjectManager
{
	static unsigned long nextGeneratedNameExt_;

public:
	SceneGraph();
	virtual ~SceneGraph();

	void clearScene();
	void update();

private:
	Node* rootNode_;
};

}

#endif
