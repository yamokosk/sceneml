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

#ifndef LCPLUGIN_H_
#define LCPLUGIN_H_

#include <tinysg/Plugin.h>

#include "RigidBodyAdapter.h"
#include "Space.h"
#include "DistanceQuery.h"

namespace lincanny
{

class Plugin : public TinySG::Plugin
{
public:
	Plugin();
	virtual ~Plugin();

	// Inhereted from tinysg::Plugin
	virtual void initialize();
	virtual void registerFactories(TinySG::Root* r);
	virtual void registerQueries(TinySG::Root* r);
	virtual void unload();

private:
	RigidBodyAdapterFactory bodyFactory_;
	SpaceFactory spaceFactory_;
	CollisionPairFactory pairFactory_;
	DistanceQuery collisionQuery_;
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
	TinySG::PluginFactory *createPluginFactory();
}

#endif /* ODEPLUGIN_H_ */
