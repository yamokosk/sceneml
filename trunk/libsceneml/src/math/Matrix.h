/*
 * Matrix.h
 *
 *  Created on: Jul 22, 2008
 *      Author: yamokosk
 */

#ifndef MATRIX_H_
#define MATRIX_H_

// External
#include <newmat/newmat.h>
#include <newmat/newmatio.h>

// Internal
#include "../Exception.h"
#include "Math.h"
#include "Quaternion.h"

namespace TinySG {

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
};

}
#endif /* MATRIX_H_ */
