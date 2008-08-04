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
 * ODEPlugin.cpp
 *
 *  Created on: Jul 30, 2008
 *      Author: yamokosk
 */

#include "ODEPlugin.h"

namespace smlode
{

ODEPlugin::ODEPlugin() :
	Plugin(),
	geomFactory_(),
	spaceFactory_(),
	collisionQuery_(),
	world_(NULL)
{
	// TODO Auto-generated constructor stub

}

ODEPlugin::~ODEPlugin()
{
	// TODO Auto-generated destructor stub
}

void ODEPlugin::initialize()
{
	dInitODE();
	world_ = dWorldCreate();
}

void ODEPlugin::registerFactories(Root* r)
{
	r->addEntityFactory( &geomFactory_ );
	r->addEntityFactory( &spaceFactory_ );
}

void ODEPlugin::registerQueries(Root* r)
{
	r->addSceneQueryFactory( &collisionQuery_ );
}

std::string ODEPlugin::getType()
{
	return "ODE";
}

void ODEPlugin::shutdown()
{
	dWorldDestroy(world_);
	dCloseODE();
}


}
