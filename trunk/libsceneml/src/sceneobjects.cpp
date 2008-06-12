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

#include "sceneobjects.h"
#include "matrix.h"

using namespace sceneml;

// --------------------------------------------------------------
//	SceneObjectBase
// --------------------------------------------------------------
/** Basic constructor */
SceneObjectBase::SceneObjectBase() :
	name_("_NODATA_"),
	validWorldPose_(false),
	proxObj_(NULL)
{
	//dTSetIdentity(tmatrix_);
}

/** Returns the objects local position vector */
void SceneObjectBase::getLocalPosition(dVector3 pos) const
{
	const dReal *tmatrix = transform_->getTMatrix();
	for (int n=0; n<3; ++n) pos[n] = tmatrix[(n*4)+3];
}

/** Returns the objects local rotation matrix */
void SceneObjectBase::getLocalRotation(dMatrix3 rot) const
{
	const dReal *tmatrix = transform_->getTMatrix();
	dRFromT(rot, tmatrix);
}

void SceneObjectBase::getLocalTransformation(dMatrix4 t) const
{	
	dAASSERT(t);
	const dReal *tmatrix = transform_->getTMatrix();
	memcpy(t, tmatrix, sizeof(dMatrix4));
}
	
/** Compute the local transformation.
  * Computes the prox transformation matrix by stepping through the objects
  * local transformation steps. */
void SceneObjectBase::computeLocalTransform()
{
	transform_->compute();
}

void SceneObjectBase::invalidate()
{
	validWorldPose_ = false;
}
