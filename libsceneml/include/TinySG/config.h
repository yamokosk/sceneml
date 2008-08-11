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

#ifndef _SCENEML_CONFIG_H_FILE_
#define _SCENEML_CONFIG_H_FILE_

#if defined(SCENEML_DLL) && defined(WIN32) && defined(_MSC_VER)
#define SCENEML_API __declspec( dllexport )
#elif !defined(SCENEML_DLL) && defined(WIN32) && defined(MSC_VER)
#define SCENEML_API __declspec( dllimport )
#else
#define SCENEML_API
#endif

//#ifndef WIN32
//#include <stdlib> 
//#endif

#ifdef _MSC_VER
#pragma warning (disable:4251)
#pragma warning (disable:4996)
#endif

#endif
