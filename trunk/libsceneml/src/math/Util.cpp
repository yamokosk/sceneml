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
 * Util.cpp
 *
 *  Created on: Jul 28, 2008
 *      Author: yamokosk
 */

#include "Util.h"

namespace TinySG
{

Quaternion QuatFromAngleAxis(Real angle, const ColumnVector& axis)
{
	if(axis.Nrows() != 3)
	{
		SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Axis parameter must have three values.");
	}

	// make sure axis is a unit vector
	Real len = sqrt(DotProduct(axis, axis));
	Real s = sin(angle/two);

	return Quaternion(cos(angle/two), axis(1)*s/len, axis(2)*s/len, axis(3)*s/len);
}

Quaternion QuatFromRotationMatrix(const Matrix& kRot)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".
	Real fTrace = kRot.trace();
	Real fRoot, w, x, y, z;

	if ( fTrace > 0.0 )
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot = sqrt(fTrace + 1.0);  // 2w
		w = 0.5*fRoot;
		fRoot = 0.5/fRoot;  // 1/(4w)
		x = (kRot(3,2)-kRot(2,3))*fRoot; //(kRot[2][1]-kRot[1][2])*fRoot;
		y = (kRot(1,3)-kRot(3,1))*fRoot; //(kRot[0][2]-kRot[2][0])*fRoot;
		z = (kRot(2,1)-kRot(1,2))*fRoot; //(kRot[1][0]-kRot[0][1])*fRoot;
	}
	else
	{
		// |w| <= 1/2
		int s_iNext[4] = { 0, 2, 3, 1 }; //static size_t s_iNext[3] = { 1, 2, 0 };
		int i = 1; //size_t i = 0;
		if ( kRot(2,2) > kRot(1,1) ) //if ( kRot[1][1] > kRot[0][0] )
			i = 2; //i = 1;
		if ( kRot(3,3) > kRot(i,i) ) //if ( kRot[2][2] > kRot[i][i] )
			i = 3; //i = 2;
		int j = s_iNext[i];
		int k = s_iNext[j];

		fRoot = sqrt(kRot(i,i)-kRot(j,j)-kRot(k,k) + one); // Math::Sqrt(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0);
		Real* apkQuat[4] = { NULL, &x, &y, &z }; //Real* apkQuat[3] = { &x, &y, &z };
		*apkQuat[i] = 0.5*fRoot;
		fRoot = 0.5/fRoot;
		w = (kRot(k,j)-kRot(j,k))*fRoot; //(kRot[k][j]-kRot[j][k])*fRoot;
		*apkQuat[j] = (kRot(j,i)+kRot(i,j))*fRoot; //(kRot[j][i]+kRot[i][j])*fRoot;
		*apkQuat[k] = (kRot(k,i)+kRot(i,k))*fRoot; //(kRot[k][i]+kRot[i][k])*fRoot;
	}
	return Quaternion(w,x,y,z);
}

Quaternion QuatFromEulerAngles(int choice, const ColumnVector& angles)
{
	if(angles.Nrows() != 3)
	{
		SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "There must be three Euler angles for conversion to quaternion.");
	}

	ColumnVector xaxis = VectorFactory::Vector3( UNIT_X );
	ColumnVector yaxis = VectorFactory::Vector3( UNIT_Y );
	ColumnVector zaxis = VectorFactory::Vector3( UNIT_Z );
	Quaternion q_ret;

	switch (choice)
	{
	case ( XYZ ):
		Quaternion q1 = QuatFromAngleAxis(angles(1), xaxis);
		Quaternion q2 = QuatFromAngleAxis(angles(2), yaxis);
		Quaternion q3 = QuatFromAngleAxis(angles(3), zaxis);
		q_ret = q3*q2*q1;
		break;
	}

	return q_ret;
}

ReturnMatrix RotFromAngleAxis(Real angle, const ColumnVector& axis)
{
	// TODO Check that axis is unit vector
	Real mx = axis(1);
	Real my = axis(2);
	Real mz = axis(3);
	Real c = cos(angle);
	Real s = sin(angle);
	Real v = 1-c;

	Matrix r = MatrixFactory::Matrix4x4( IDENTITY );
	r(1,1) = mx*mx*v+c;
	r(1,2) = mx*my*v-mz*s;
	r(1,3) = mx*mz*v+my*s;

	r(2,1) = mx*my*v+mz*s;
	r(2,2) = my*my*v+c;
	r(2,3) = my*mz*v-mx*s;

	r(3,1) = mx*mz*v-my*s;
	r(3,2) = my*mz*v+mx*s;
	r(3,3) = mz*mz*v+c;
	r.Release(); return r;
}

ReturnMatrix RotFromEulerSequence(int seqType, Real tx, Real ty, Real tz)
{
	Real r11, r12, r13, r21, r22, r23, r31, r32, r33;
	Real cx = cos(tx), cy = cos(ty), cz = cos(tz);
	Real sx = sin(tx), sy = sin(ty), sz = sin(tz);

	switch (seqType)
	{
	case XYZ:
		r11 = cy*cz;
		r12 = cx*sz+sx*sy*cz;
		r13 = sx*sz-cx*sy*cz;

		r21 = -cy*sz;
		r22 = cx*cz-sx*sy*sz;
		r23 = sx*cz+cx*sy*sz;

		r31 = sy;
		r32 = -sx*cy;
		r33 = cx*cy;
		break;
	case ZXY:
		r11 = cz*cy-sz*sx*sy;
		r12 = cy*sz+sy*sx*cz;
		r13 = -sy*cx;

		r21 = -cx*sz;
		r22 = cz*cx;
		r23 = sx;

		r31 = sy*cz+cy*sx*sz;
		r32 = sz*sy-sx*cz*cy;
		r33 = cy*cx;
		break;
	default:
		SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid Euler sequence.");
		break;
	}

	Matrix r = MatrixFactory::Matrix4x4( IDENTITY );
	r(1,1) = r11;
	r(1,2) = r12;
	r(1,3) = r13;

	r(2,1) = r21;
	r(2,2) = r22;
	r(2,3) = r23;

	r(3,1) = r31;
	r(3,2) = r32;
	r(3,3) = r33;
	r.Release(); return r;
}

} // End namespace TinySG


ReturnMatrix operator* (const TinySG::Quaternion& q, const ColumnVector& v)
{
	if(v.Nrows() != 3)
	{
		SML_EXCEPT(TinySG::Exception::ERR_INVALIDPARAMS, "Vector must be length 3.");
	}

	// nVidia SDK implementation - from OGRE implementation
	ColumnVector qvec(3); qvec << q.R_component_2() << q.R_component_3() << q.R_component_4();
	ColumnVector uv = crossproduct(qvec,v);
	ColumnVector uuv = crossproduct(qvec,uv);
	uv *= (TinySG::two * q.real());
	uuv *= TinySG::two;

	ColumnVector ret = v + uv + uuv;
	ret.Release();
	return ret;
}
