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
 * Vector.h
 *
 *  Created on: Jul 22, 2008
 *      Author: yamokosk
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <tinysg/Exception.h>
#include <tinysg/Math.h>
#include <newmat/newmat.h>
#include <newmat/newmatio.h>

namespace TinySG {

struct VectorFactory
{
	static ReturnMatrix Vector3(int choice)
	{
		ColumnVector ret(3);
		switch (choice)
		{
		case UNIT_X:
			ret << one << zero << zero;
			break;
		case UNIT_Y:
			ret << zero << one << zero;
			break;
		case UNIT_Z:
			ret << zero << zero << one;
			break;
		case ZERO:
			ret << zero << zero << zero;
			break;
		case ONES:
			ret << one << one << one;
			break;
		default:
			SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid factory parameter.");
		}
		ret.Release(); return ret;
	};
	static ReturnMatrix Vector3(Real x, Real y, Real z)
	{
		ColumnVector ret(3);
		ret << x << y << z;
		ret.Release(); return ret;
	};
	static ReturnMatrix Vector4(int choice)
	{
		ColumnVector ret(4);
		switch (choice)
		{
		case UNIT_X:
			ret << one << zero << zero << one;
			break;
		case UNIT_Y:
			ret << zero << one << zero << one;
			break;
		case UNIT_Z:
			ret << zero << zero << one << one;
			break;
		case ZERO:
			ret << zero << zero << zero << one;
			break;
		case ONES:
			ret << one << one << one << one;
			break;
		default:
			SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid factory parameter.");
		}
		ret.Release(); return ret;
	};
};

}
#endif /* VECTOR_H_ */
