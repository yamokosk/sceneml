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
 * Plugin.h
 *
 *  Created on: Jul 30, 2008
 *      Author: yamokosk
 */

#ifndef _ODE_PLUGIN_H_
#define _ODE_PLUGIN_H_

// TinySG
#include <tinysg/config.h>
#include <tinysg/Plugin.h>

// ODE library
#include <ode/ode.h>

#include "Geom.h"
#include "Space.h"
#include "CollisionQuery.h"

namespace sgode
{

class Plugin : public TinySG::Plugin
{
public:
	Plugin();
	virtual ~Plugin();

	// Inhereted from TinySG::Plugin
	virtual void initialize();
	virtual void registerFactories(TinySG::Root* r);
	virtual void registerQueries(TinySG::Root* r);
	virtual void unload();

private:
	GeomFactory geomFactory_;
	SpaceFactory spaceFactory_;
	CollisionPairFactory pairFactory_;
	CollisionQuery collisionQuery_;
	dWorldID world_;
};

// Used for dynamically loading driver
struct Factory : public TinySG::PluginFactory
{
	TinySG::Plugin* createPlugin() const
	{
		return new Plugin();
	}
};

}

// Used for dynamically loading driver
extern "C" {
	SOEXPORT TinySG::PluginFactory* createPluginFactory();
}

#endif /* ODEPLUGIN_H_ */
