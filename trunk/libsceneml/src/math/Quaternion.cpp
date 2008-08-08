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

namespace tinysg {

Quaternion QuaternionFactory::ZERO(zero, zero, zero, zero);
Quaternion QuaternionFactory::IDENTITY(one, zero, zero, zero);
/*Quaternion Quat(int choice)
{
	switch (choice){
	case IDENTITY:
		return Quaternion(one, zero, zero, zero);
	case ZERO:
		return Quaternion(zero, zero, zero, zero);
	default:
		SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid factory parameter.");
	}
}*/

Quaternion inverse(const Quaternion& q)
{
	return Quaternion(q.real(), -q.R_component_2(), -q.R_component_3(), -q.R_component_4() );
}

}
