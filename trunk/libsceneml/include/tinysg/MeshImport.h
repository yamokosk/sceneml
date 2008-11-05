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

// Forward declaration
class TriMesh;

namespace TinySG
{

void importOBJ(const std::string& filename, TriMesh* m, float sx=1.0f, float sy=1.0f, float sz=1.0f);
void importSTL(const std::string& filename, TriMesh* m, float sx=1.0f, float sy=1.0f, float sz=1.0f)

}

#endif /* MESHIMPORT_H_ */
