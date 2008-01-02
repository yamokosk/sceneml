/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/
 
#ifndef _XODE_MATRIX_H_FILE_
#define _XODE_MATRIX_H_FILE_

#include <ode/ode.h>

namespace xode {

void dTSetIdentity(dMatrix4 T);
void dTFromTrans(dMatrix4 T, dReal x, dReal y, dReal z);
void dTFromAxisAndAngle (dMatrix4 T, dReal ax, dReal ay, dReal az, dReal angle);
void dTfromQ (dMatrix4 T, const dQuaternion q);
void dTFromEuler123(dReal* T, dReal x, dReal y, dReal z);
void dTFromRAndPos(dMatrix4 T, const dMatrix3 rot, const dVector3 pos);
void dRFromT(dMatrix3 R, const dMatrix4 T);
void dTPrint(dMatrix4 T);
void dRPrint(dMatrix3 T);
void dVPrint(dVector3 V);
};

#endif
