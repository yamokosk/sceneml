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

#ifdef WIN32
#	include <Windows.h>
#else
#	include <dlfcn.h>
#endif

#include <sstream>
#include <iostream>
#include "DynamicLibrary.h"

using namespace std;

namespace TinySG {

DynamicLibrary::DynamicLibrary(void * handle) : handle_(handle)
{
}

DynamicLibrary::~DynamicLibrary()
{
	if (handle_)
	{
#ifndef WIN32
		::dlclose(handle_);
#else
		::FreeLibrary((HMODULE)handle_);
#endif
	}
}

DynamicLibrary * DynamicLibrary::load(const std::string& name, std::string & errorString)
{
	if (name.empty())
	{
		errorString = "Empty path.";
		return NULL;
	}

	void * handle = NULL;

#ifdef WIN32
	handle = ::LoadLibraryA(name.c_str());
	if (handle == NULL)
	{
		DWORD errorCode = ::GetLastError();
		std::stringstream ss;
		ss << std::string("LoadLibrary(") << name
		 << std::string(") Failed. errorCode: ")
		 << errorCode;
		errorString = ss.str();
		return NULL;
	}
#else
	handle = ::dlopen(name.c_str(), RTLD_NOW);
	if (!handle)
	{
		std::string dlErrorString;
		const char *zErrorString = ::dlerror();
		if (zErrorString) dlErrorString = zErrorString;
		errorString += "Failed to load \"" + name + '"';
		if(dlErrorString.size()) errorString += ": " + dlErrorString;
		return NULL;
	}
#endif
	return new DynamicLibrary(handle);
}

void* DynamicLibrary::getSymbol(const std::string & symbol)
{
	if (!handle_) return NULL;

#ifdef WIN32
	return ::GetProcAddress((HMODULE)handle_, symbol.c_str());
#else
	return ::dlsym(handle_, symbol.c_str());
#endif
}


}
