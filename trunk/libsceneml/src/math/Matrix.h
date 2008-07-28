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
		Real ctd2 = q.real();
		ColumnVector axis(3); axis << q.R_component_2() << q.R_component_3() << q.R_component_4();
		Real std2 = axis.NormFrobenius();
		axis /= std2;
		Real theta = two * atan2(std2, ctd2);

		Real ct = cos(theta);
		Real st = sin(theta);
		Real vt = one - ct;
		Real mx = axis(1);
		Real my = axis(2);
		Real mz = axis(3);

		Matrix ret(3,3); ret = zero;

		ret(1,1) = pow(mx,2)*vt + ct;
		ret(1,2) = mx*my*vt - mz*st;
		ret(1,3) = mx*mz*vt + my*st;

		ret(2,1) = mx*my*vt + mz*st;
		ret(2,2) = pow(my,2)*vt + ct;
		ret(2,3) = my*mz*vt - mx*st;

		ret(3,1) = mx*mz*vt - my*st;
		ret(3,2) = my*mz*vt + mx*st;
		ret(3,3) = pow(mz,2)*vt + ct;
		ret.Release(); return ret;
	}
};

}

}
#endif /* MATRIX_H_ */
