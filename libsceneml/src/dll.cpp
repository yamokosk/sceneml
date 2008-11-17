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
 * Code taken from the Orca-Robotics Project
 * http://orca-robotics.sf.net/
 */

#include <iostream>
#include <sstream>

#ifndef WIN32
#	define LIBRARY_EXTENSION ".so"
#   include <dlfcn.h>
#else
#	define LIBRARY_EXTENSION ".dll"
#   include <windows.h>
#endif

#include <tinysg/dll.h>

using namespace std;

namespace hydrodll {

DynamicallyLoadedLibrary::DynamicallyLoadedLibrary( const std::string &libName )
    : libName_(libName)
{
	std::stringstream fullLibName;

#ifndef WIN32
	fullLibName << "lib" << libName << LIBRARY_EXTENSION;
#else
	fullLibName << libName << LIBRARY_EXTENSION;
#endif

#ifndef WIN32
    libHandle_ = dlopen( fullLibName.str().c_str(), RTLD_NOW );
	if ( libHandle_ == NULL )
    {
        stringstream ss;
        ss << "Failed to load '"<<libName<<"': "<<dlerror();

		throw DynamicLoadException( ss.str() );
    }
#else
	libHandle_ = LoadLibrary( fullLibName.str().c_str() );
	if ( libHandle_ == NULL )
	{
		stringstream ss;
		ss << "Failed to load'"<<libName<<"': " << GetLastError();
		throw DynamicLoadException( ss.str() );
	}
#endif

}

DynamicallyLoadedLibrary::~DynamicallyLoadedLibrary()
{
#ifndef WIN32
    dlclose( libHandle_ );
#else
	FreeLibrary( (HMODULE)libHandle_ );
#endif
}

}
