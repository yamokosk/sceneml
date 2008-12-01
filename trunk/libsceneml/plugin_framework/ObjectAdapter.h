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
 * ObjectAdapter.h
 *
 *  Created on: Dec 1, 2008
 *      Author: yamokosk
 */

#ifndef OBJECTADAPTER_H_
#define OBJECTADAPTER_H_

#include "Plugin.h"

// This interface is used to adapt C plugin objects to C++ plugin objects.
// It must be passed to the PluginManager::createObject() function.
struct IObjectAdapter
{
  virtual ~IObjectAdapter() {}
  virtual void * adapt(void * object, PF_DestroyFunc df) = 0;
};

// This template should be used if the object model implements the
// dual C/C++ object design pattern. Otherwise you need to provide
// your own object adapter class that implements IObjectAdapter
template<typename T, typename U>
struct ObjectAdapter : public IObjectAdapter
{
  virtual void * adapt(void * object, PF_DestroyFunc df)
  {
    return new T((U *)object, df);
  }
};

#endif /* OBJECTADAPTER_H_ */
