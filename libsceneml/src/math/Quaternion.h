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

#include <Exception.h>

#include <math/Math.h>

#include <boost/math/quaternion.hpp>

#include <newmat/newmatap.h>                // need matrix applications
#include <newmat/newmatio.h>                // need matrix output routines

namespace tinysg {

typedef ::boost::math::quaternion<Real> Quaternion;

struct QuaternionFactory
{
	static Quaternion ZERO;
	static Quaternion IDENTITY;
};

Quaternion inverse(const Quaternion& q);

} // tinysg



#endif
