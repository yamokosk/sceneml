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
#include <tinysg/Exception.h>
#include <tinysg/Math.h>
#include <tinysg/Quaternion.h>

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
