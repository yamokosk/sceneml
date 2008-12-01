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

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "Plugin.h"

namespace TinySG
{

class DynamicLibrary;
class IObjectAdapter;

class PluginManager
{
	typedef std::map<std::string, boost::shared_ptr<DynamicLibrary> > DynamicLibraryMap;
	typedef std::vector<PF_ExitFunc> ExitFuncVec;
	typedef std::vector<PF_RegisterParams> RegistrationVec;

public:
	typedef std::map<std::string, PF_RegisterParams> RegistrationMap;

	static PluginManager & getInstance();
	static int initializePlugin(PF_InitFunc initFunc);
	int loadAll(const std::string & pluginDirectory, PF_InvokeServiceFunc func = NULL);
	int loadByPath(const std::string & path);

	void* createObject(const std::string & objectType, IObjectAdapter & adapter);

	template<class T>
	T* createObjectAndCast(const std::string & objectType)
	{
		return dynamic_cast<T*>( createObject(objectType) );
	}

	int shutdown();
	static int registerObject(const char* nodeType, const PF_RegisterParams * params);
	const RegistrationMap & getRegistrationMap();
	PF_PlatformServices & getPlatformServices();

private:
	~PluginManager();
	PluginManager();
	PluginManager(const PluginManager &);

	DynamicLibrary* loadLibrary(const std::string & path, std::string & errorString);
private:
	bool                inInitializePlugin_;
	PF_PlatformServices platformServices_;
	DynamicLibraryMap   dynamicLibraryMap_;
	ExitFuncVec         exitFuncVec_;

	RegistrationMap     tempExactMatchMap_;   // register exact-match object types
	RegistrationVec     tempWildCardVec_;     // wild card ('*') object types

	RegistrationMap     exactMatchMap_;   // register exact-match object types
	RegistrationVec     wildCardVec_;     // wild card ('*') object types
};

} // End namespace TinySG


#endif
