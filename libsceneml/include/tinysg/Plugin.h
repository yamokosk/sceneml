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
 * Plugin.h
 *
 *  Created on: Jul 30, 2008
 *      Author: yamokosk
 */

#ifndef PLUGIN_H_
#define PLUGIN_H_

// Standard includes
#include <string>

namespace TinySG
{

// Forward declarations
class Root;

class Plugin
{
public:
	Plugin(const std::string& type) : type_(type) {};
	virtual ~Plugin();

	// Functions to be implemented by child classes
	virtual void initialize() = 0;
	virtual void registerFactories(Root* r) = 0;
	virtual void unload() = 0;

	// Returns plugin type.. needs to be a unique identifier
	const std::string& getType();

private:
	const std::string type_;
};

}

#endif /* PLUGIN_H_ */
