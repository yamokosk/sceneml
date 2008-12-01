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
 * MeshImport.h
 *
 *  Created on: Nov 3, 2008
 *      Author: yamokosk
 */

#ifndef MESHIMPORT_H_
#define MESHIMPORT_H_

#include <string>

namespace TinySG
{

// Forward declaration
class TriMesh;

TriMesh* importOBJ(const std::string& filename, float sx, float sy, float sz, std::string& errmsg);
TriMesh* importSTL(const std::string& filename, float sx, float sy, float sz, std::string& errmsg);

}

#endif /* MESHIMPORT_H_ */
