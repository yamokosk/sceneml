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
 * ODEPlugin.h
 *
 *  Created on: Jul 30, 2008
 *      Author: yamokosk
 */

#ifndef ODEPLUGIN_H_
#define ODEPLUGIN_H_

// SceneML
#include <SceneML.h>

// ODE library
#include <ode/ode.h>

#include "ODEGeom.h"
#include "ODESpace.h"
#include "CollisionQuery.h"

namespace smlode
{

class ODEPlugin : public sml::Plugin
{
public:
	ODEPlugin();
	virtual ~ODEPlugin();

	virtual void initialize();
	virtual void registerFactories(Root* r);
	virtual void registerQueries(Root* r);
	virtual std::string getType();
	virtual void shutdown();

	GeomFactory geomFactory_;
	SpaceFactory spaceFactory_;
	CollisionQuery collisionQuery_;
	dWorldID world_;
};

}

#endif /* ODEPLUGIN_H_ */
