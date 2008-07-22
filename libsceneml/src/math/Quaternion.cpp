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

#include "Quaternion.h"

namespace sml {

namespace math {

Quaternion inverse(const Quaternion& q)
{
	return Quaternion(q.real(), -q.R_component_2(), -q.R_component_3(), -q.R_component_4() );
}

}

}

ReturnMatrix operator* (const sml::math::Quaternion& q, const ColumnVector& v)
{
	if(v.Nrows() != 3)
	{
		SML_EXCEPT(sml::Exception::ERR_INVALIDPARAMS, "Vector must be length 3.");
	}

	// nVidia SDK implementation - from OGRE implementation
	ColumnVector qvec(3); qvec << q.R_component_2() << q.R_component_3() << q.R_component_4();
	ColumnVector uv = crossproduct(qvec,v);
	ColumnVector uuv = crossproduct(qvec,uv);
	uv *= (sml::math::two * q.real());
	uuv *= sml::math::two;

	ColumnVector ret = v + uv + uuv;
	ret.Release();
	return ret;
}
