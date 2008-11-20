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

#include <tinysg/Primitives.h>

namespace TinySG
{

void createSphere (TriMesh* mesh, float radius, unsigned int quality)
{
	// TODO implement createSphere()
	SML_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Not implemented.");
}

void createCapsule(TriMesh*, float len, float radius, unsigned int quality)
{
	// TODO implement createCapsule()
	SML_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Not implemented.");
}

void createCylinder(TriMesh*, float len, float radius, unsigned int quality)
{
	// TODO implement createCylinder()
	SML_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Not implemented.");
}

void createBox(TriMesh* mesh, float lx, float ly, float lz)
{
	// Place the mesh coordinate system in center of box.. done for historical reasons
	lx *= 0.5;
	ly *= 0.5;
	lz *= 0.5;

	mesh->init(8, 12);

	// Create vertices
	mesh->addVertex(-lx, -ly, -lz);
	mesh->addVertex(lx, -ly, -lz);
	mesh->addVertex(lx,  ly, -lz);
	mesh->addVertex(-lx,  ly, -lz);
	mesh->addVertex(-lx, -ly,  lz);
	mesh->addVertex(lx, -ly,  lz);
	mesh->addVertex(lx,  ly,  lz);
	mesh->addVertex(-lx,  ly,  lz);

	// Create faces
	mesh->addFace(1, 0, 3);
	mesh->addFace(1, 3, 2);
	mesh->addFace(4, 5, 6);
	mesh->addFace(4, 6, 7);
	mesh->addFace(6, 5, 1);
	mesh->addFace(6, 1, 2);
	mesh->addFace(4, 7, 3);
	mesh->addFace(4, 3, 0);
	mesh->addFace(3, 6, 2);
	mesh->addFace(3, 7, 6);
	mesh->addFace(1, 5, 4);
	mesh->addFace(1, 4, 0);
}

} // End namespace



