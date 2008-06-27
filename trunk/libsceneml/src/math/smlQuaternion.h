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

#ifndef SML_QUATERNION_H
#define SML_QUATERNION_H

#include <math/smlMath.h>
#include <smlException.h>
#include <boost/math/quaternion.hpp>

#include <newmatap.h>                // need matrix applications
#include <newmatio.h>                // need matrix output routines

namespace sml {
	namespace math {

		typedef ::boost::math::quaternion<Real> Quaternion;

		/*struct Quaternions
		{
			// Common quaternions
			static const Quaternion zero;
			static const Quaternion identity;
		};*/

		//const Quaternion Quaternions::zero(sml::math::zero,sml::math::zero,sml::math::zero,sml::math::zero);
		//const Quaternion Quaternions::identity(sml::math::one,sml::math::zero,sml::math::zero,sml::math::zero);
		
		// Factory methods
		static Quaternion quaternionFromAngleAxis(const Real angle, const ColumnVector& axis)
		{
			if(axis.Nrows() != 3)
			{
				// TODO: throw error
				return Quaternion(1.,0.,0.,0.);
			}

			// make sure axis is a unit vector
			Real len = sqrt(DotProduct(axis, axis));
			Real s = sin(angle/2.);
			
			return Quaternion(cos(angle/2.), axis(1)*s/len, axis(2)*s/len, axis(3)*s/len);
		}
		
		//-----------------------------------------------------------------------
		ReturnMatrix operator* (const Quaternion& q, const ColumnVector& v)
		{
			if(v.Nrows() != 3)
			{
				// TODO: throw error
				ColumnVector ret(3); ret = math::zero;
				ret.Release();
				return ret;
			}
			
			// nVidia SDK implementation - from OGRE implementation
			ColumnVector(3) qvec; qvec << q.R_component_2() << q.R_component_3() << q.R_component_4();
			ColumnVector uv = qvec.crossProduct(v);
			ColumnVector uuv = qvec.crossProduct(uv);
			uv *= (math::two * w);
			uuv *= math::two;

			ColumnVector ret = v + uv + uuv;
			ret.Release();
			return ret;
		}
		
	} // sml::math
} // sml


#endif
