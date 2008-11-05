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
 * Plugin.cpp
 *
 *  Created on: Jul 30, 2008
 *      Author: yamokosk
 */

#include "Plugin.h"

namespace sgode
{

Plugin::Plugin() :
	tinysg::Plugin("ODE"),
	geomFactory_(),
	spaceFactory_(),
	collisionQuery_(),
	world_(NULL)
{
	// TODO Auto-generated constructor stub

}

Plugin::~Plugin()
{
	// TODO Auto-generated destructor stub
}

void Plugin::initialize()
{
	dInitODE();
	world_ = dWorldCreate();
}

void Plugin::registerFactories(Root* r)
{
	r->addEntityFactory( &geomFactory_ );
	r->addEntityFactory( &spaceFactory_ );
	r->addSceneQueryFactory( &collisionQuery_ );
}

void Plugin::unload()
{
	dWorldDestroy(world_);
	dCloseODE();
}


}
