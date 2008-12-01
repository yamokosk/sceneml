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

#ifndef PF_PLUGIN_H
#define PF_PLUGIN_H


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Allows plugins to declare to the plugin manager if they are implemented in C or C++
 */
typedef enum PF_ProgrammingLanguage
{
	PF_ProgrammingLanguage_C,
	PF_ProgrammingLanguage_CPP
} PF_ProgrammingLanguage;

struct PF_PlatformServices;

/*
 * Abstract struct that is passed to created plugin objects
 */
typedef struct PF_ObjectParams
{
	const char* objectType;
	const struct PF_PlatformServices* platformServices;
} PF_ObjectParams;

/*
 * Used to negotiate versioning and make sure that the plugin manager loads only plugins with compatible version.
 */
typedef struct PF_PluginAPI_Version
{
	unsigned int major;
	unsigned int minor;
} PF_PluginAPI_Version;

/*
 * The functions pointer definitions PF_CreateFunc and PF_DestroyFunc (implemented by the plugin)
 * allow the plugin manager to create and destroy plugin objects (each plugin registers such
 * functions with the plugin manager)
 */
typedef void * (*PF_CreateFunc)(PF_ObjectParams *);
typedef int (*PF_DestroyFunc)(void *);

/*
 * The PF_RegisterParams struct contains all the information that a plugin must provide to the
 * plugin manager upon initialization (version, create/destroy functions, and programming language).
 */
typedef struct PF_RegisterParams
{
	PF_PluginAPI_Version version;
	PF_CreateFunc createFunc;
	PF_DestroyFunc destroyFunc;
	PF_ProgrammingLanguage programmingLanguage;
} PF_RegisterParams;

/*
 * The PF_RegisterFunc (implemented by the plugin manager) allows each plugin to register a
 * PF_RegisterParams struct for each object type it supports. Note that this scheme allows a plugin
 * to register different versions of an object and multiple object types.
 */
typedef int (*PF_RegisterFunc)(const char* nodeType, const PF_RegisterParams * params);
/*
 * The PF_InvokeService function pointer definition is a generic function that plugins can use to invoke
 * services of the main system like logging, event notification and error reporting. The signature
 * includes the service name and an opaque pointer to a parameters struct. The plugins should know about
 * available services and how to invoke them (or you can implement service discovery if you wish using PF_InvokeService).
 */
typedef int (*PF_InvokeServiceFunc)(const char* serviceName, void * serviceParams);

/*
 * The PF_PlatformServices struct aggregates all the services I just mentioned that the platform
 * provides to plugin (version, registering objects and the invoke service function). This struct is
 * passed to each plugin at initialization time.
 */
typedef struct PF_PlatformServices
{
	PF_PluginAPI_Version version;
	PF_RegisterFunc registerObject;
	PF_InvokeServiceFunc invokeService;
} PF_PlatformServices;

/*
 * Definition of the plugin exit function pointer (implemented by the plugin).
 */
typedef int (*PF_ExitFunc)();

/*
 * Definition of the plugin initialization function pointer.
 */
typedef PF_ExitFunc (*PF_InitFunc)(const PF_PlatformServices *);

#ifndef PLUGIN_API
#	ifdef WIN32
#		define PLUGIN_API __declspec(dllimport)
#	else
#		define PLUGIN_API
#	endif
#endif

extern
#ifdef __cplusplus
"C"
#endif
PLUGIN_API PF_ExitFunc PF_initPlugin(const PF_PlatformServices * params);


#ifdef __cplusplus
}
#endif


#endif /* PF_PLUGIN_H */
