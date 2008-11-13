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
 * Version.cpp
 *
 *  Created on: Nov 13, 2008
 *      Author: yamokosk
 */

#include <tinysg/Version.h>

namespace TinySG
{

Version::Version( const std::string& verS ) :
	major(0), minor(0), patch(0)
{
	// verS = '1.2.4'
	unsigned int *vptrs[3];
	vptrs[0] = &major;
	vptrs[1] = &minor;
	vptrs[2] = &patch;
	std::istringstream stream(verS);

	for (unsigned int n=0; n < 3; ++n)
	{
		std::string numS;
		getline(stream, numS, '.');
		std::istringstream numStream(numS);
		numStream >> *vptrs[n];
	}
}

Version::Version( unsigned int ma, unsigned int mi, unsigned int p ) :
	major(ma), minor(mi), patch(p)
{}

std::string Version::toString()
{
	std::stringstream ss;
	ss << major << "." << minor << "." << patch;
	return ss.str();
}

bool operator<= (const Version& v1, const Version& v2)
{
	if ( v1.major < v2.major )
	{
		return true;
	}

	if ( (v1.major == v2.major) && (v1.minor <= v2.minor) )
	{
		return true;
	}

	return false;
}

bool operator> (const Version& v1, const Version& v2)
{
	return !( v1 <= v2 );
}

bool operator== (const Version& v1, const Version& v2)
{
	return (v1.major == v2.major) && (v1.minor == v2.minor) && (v1.patch == v2.patch);
}

}
