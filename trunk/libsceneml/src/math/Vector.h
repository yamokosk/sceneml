/*
 * Vector.h
 *
 *  Created on: Jul 22, 2008
 *      Author: yamokosk
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <Exception.h>
#include <math/Math.h>
#include <newmat/newmat.h>

namespace sml {

namespace math {

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

}
#endif /* VECTOR_H_ */
