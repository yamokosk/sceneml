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

#include <xode/matrix.h>
#include <ode/rotation.h>
#include <iostream>

#define _T(i,j) T[(i)*4+(j)]

#define SET_4x4_IDENTITY \
  _T(0,0) = REAL(1.0); \
  _T(0,1) = REAL(0.0); \
  _T(0,2) = REAL(0.0); \
  _T(0,3) = REAL(0.0); \
  _T(1,0) = REAL(0.0); \
  _T(1,1) = REAL(1.0); \
  _T(1,2) = REAL(0.0); \
  _T(1,3) = REAL(0.0); \
  _T(2,0) = REAL(0.0); \
  _T(2,1) = REAL(0.0); \
  _T(2,2) = REAL(1.0); \
  _T(2,3) = REAL(0.0); \
  _T(3,0) = REAL(0.0); \
  _T(3,1) = REAL(0.0); \
  _T(3,2) = REAL(0.0); \
  _T(3,3) = REAL(1.0);

namespace xode {

void dTSetIdentity(dMatrix4 T)
{
	dAASSERT(T);
	SET_4x4_IDENTITY;
}

void dTFromTrans(dMatrix4 T, dReal x, dReal y, dReal z)
{
	dTSetIdentity(T);
	_T(0,3) = x;
	_T(1,3) = y;
	_T(2,3) = z;
}

void dTFromAxisAndAngle (dMatrix4 T, dReal ax, dReal ay, dReal az, dReal angle)
{
	dAASSERT (T);
	dQuaternion q;
	dQFromAxisAndAngle (q,ax,ay,az,angle);
	dTfromQ (T,q);
}

void dTfromQ (dMatrix4 T, const dQuaternion q)
{
	dAASSERT (q && R);
	// q = (s,vx,vy,vz)
	dReal qq1 = 2*q[1]*q[1];
	dReal qq2 = 2*q[2]*q[2];
	dReal qq3 = 2*q[3]*q[3];
	_T(0,0) = 1 - qq2 - qq3;
	_T(0,1) = 2*(q[1]*q[2] - q[0]*q[3]);
	_T(0,2) = 2*(q[1]*q[3] + q[0]*q[2]);
	_T(0,3) = REAL(0.0);
	_T(1,0) = 2*(q[1]*q[2] + q[0]*q[3]);
	_T(1,1) = 1 - qq1 - qq3;
	_T(1,2) = 2*(q[2]*q[3] - q[0]*q[1]);
	_T(1,3) = REAL(0.0);
	_T(2,0) = 2*(q[1]*q[3] - q[0]*q[2]);
	_T(2,1) = 2*(q[2]*q[3] + q[0]*q[1]);
	_T(2,2) = 1 - qq1 - qq2;
	_T(2,3) = REAL(0.0);
	_T(3,0) = REAL(0.0);
	_T(3,1) = REAL(0.0);
	_T(3,2) = REAL(0.0);
	_T(3,3) = REAL(1.0);	
}

void dTFromEuler123(dMatrix4 T, dReal x, dReal y, dReal z)
{
	dReal cx,cy,cz,sx,sy,sz;
	dAASSERT (T);
	cx = cos(x);
	cy = cos(y);
	cz = cos(z);
	sx = sin(x);
	sy = sin(y);
	sz = sin(z);
	
	_T(0,0) = cy*cz;
	_T(0,1) = -cy*sz;
	_T(0,2) = sy;
	_T(0,3) = REAL(0.0);
	_T(1,0) = sx*sy*cz+cx*sz;
	_T(1,1) = -sx*sy*sz+cx*cz;
	_T(1,2) = -sx*cy;
	_T(1,3) = REAL(0.0);
	_T(2,0) = -cx*sy*cz+sx*sz;
	_T(2,1) = cx*sy*sz+sx*cz;
	_T(2,2) = cx*cy;
	_T(2,3) = REAL(0.0);
	_T(3,0) = REAL(0.0);
	_T(3,1) = REAL(0.0);
	_T(3,2) = REAL(0.0);
	_T(3,3) = REAL(1.0);
}

void dTFromRAndPos(dMatrix4 T, const dMatrix3 rot, const dVector3 pos)
{
	dAASSERT(T);
	memcpy(T,	rot, sizeof(dMatrix3));
	_T(0,3) = pos[0];
	_T(1,3) = pos[1];
	_T(2,3) = pos[2];
	_T(3,0) = REAL(0.0);
	_T(3,1) = REAL(0.0);
	_T(3,2) = REAL(0.0);
	_T(3,3) = REAL(1.0);
}

void dRFromT(dMatrix3 R, const dMatrix4 T)
{
	dAASSERT(R);
	memcpy(R, T, sizeof(dMatrix3));
}

void dTPrint(dMatrix4 T)
{
	dAASSERT(T);
	printf("\t[%g, %g, %g, %g]\n", _T(0,0), _T(0,1), _T(0,2), _T(0,3));
	printf("\t[%g, %g, %g, %g]\n", _T(1,0), _T(1,1), _T(1,2), _T(1,3));
	printf("\t[%g, %g, %g, %g]\n", _T(2,0), _T(2,1), _T(2,2), _T(2,3));
	printf("\t[%g, %g, %g, %g]\n", _T(3,0), _T(3,1), _T(3,2), _T(3,3));
}

void dRPrint(dMatrix3 T)
{
	dAASSERT(T);
	printf("\t[%g, %g, %g]\n", _T(0,0), _T(0,1), _T(0,2));
	printf("\t[%g, %g, %g]\n", _T(1,0), _T(1,1), _T(1,2));
	printf("\t[%g, %g, %g]\n", _T(2,0), _T(2,1), _T(2,2));
}

void dVPrint(dVector3 V)
{
	dAASSERT(V);
	printf("\t[%g, %g, %g]\n", V[0], V[1], V[2]);
}

};
