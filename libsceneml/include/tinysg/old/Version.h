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
 * Version.h
 *
 *  Created on: Nov 13, 2008
 *      Author: yamokosk
 */

#ifndef VERSION_H_
#define VERSION_H_

#define TINYSG_MAJOR_VERSION 1
#define TINYSG_MINOR_VERSION 0
#define TINYSG_PATCH_VERSION 0

#include <string>
#include <sstream>

namespace TinySG
{

struct Version
{
	Version( const std::string& verS );
	Version( unsigned int ma, unsigned int mi, unsigned int p );

	std::string toString();
	static Version current() {return Version(TINYSG_MAJOR_VERSION, TINYSG_MINOR_VERSION, TINYSG_PATCH_VERSION);}

	unsigned int major;
	unsigned int minor;
	unsigned int patch;
};

bool operator<= (const Version& v1, const Version& v2);
bool operator> (const Version& v1, const Version& v2);
bool operator== (const Version& v1, const Version& v2);

}

#endif /* VERSION_H_ */
