/*
 * Matrix.h
 *
 *  Created on: Jul 22, 2008
 *      Author: yamokosk
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <Exception.h>
#include <math/Math.h>
#include <math/Quaternion.h>
#include <newmat/newmat.h>

namespace sml {

namespace math {

struct MatrixFactory
{
	static ReturnMatrix Matrix3x3(int choice)
	{
		Matrix ret(3,3);
		switch (choice)
		{
		case IDENTITY:
			ret = IdentityMatrix(3);
			break;
		case ZERO:
			ret = zero;
			break;
		default:
			SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid factory parameter.");
		}
		ret.Release(); return ret;
	};
	static ReturnMatrix Matrix4x4(int choice)
	{
		Matrix ret(4,4);
		switch (choice)
		{
		case IDENTITY:
			ret = IdentityMatrix(4);
			break;
		case ZERO:
			ret = zero;
			break;
		default:
			SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid factory parameter.");
		}
		ret.Release(); return ret;
	};
	static ReturnMatrix FromQuaternion(const Quaternion& q)
	{
		Real w = q.real();
		Real x = q.R_component_2();
		Real y = q.R_component_3();
		Real z = q.R_component_4();

		Real fTx  = 2.0*x;
		Real fTy  = 2.0*y;
		Real fTz  = 2.0*z;
		Real fTwx = fTx*w;
		Real fTwy = fTy*w;
		Real fTwz = fTz*w;
		Real fTxx = fTx*x;
		Real fTxy = fTy*x;
		Real fTxz = fTz*x;
		Real fTyy = fTy*y;
		Real fTyz = fTz*y;
		Real fTzz = fTz*z;

		Matrix ret(3,3);
		ret(1,1) = 1.0-(fTyy+fTzz);
		ret(1,2) = fTxy-fTwz;
		ret(1,3) = fTxz+fTwy;
		ret(2,1) = fTxy+fTwz;
		ret(2,2) = 1.0-(fTxx+fTzz);
		ret(2,3) = fTyz-fTwx;
		ret(3,1) = fTxz-fTwy;
		ret(3,2) = fTyz+fTwx;
		ret(3,3) = 1.0-(fTxx+fTyy);
		ret.Release(); return ret;
	};
};

}

}
#endif /* MATRIX_H_ */
