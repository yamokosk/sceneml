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
/*
 * Util.h
 *
 *  Created on: Jul 28, 2008
 *      Author: yamokosk
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <math/Math.h>
#include <math/Vector.h>
#include <math/Matrix.h>
#include <math/Quaternion.h>

namespace sml
{
	// Quaternion construction utility functions
	Quaternion QuatFromAngleAxis(Real angle, const ColumnVector& axis);
	Quaternion QuatFromRotationMatrix(const Matrix& kRot);
	Quaternion QuatFromEulerAngles(int choice, const ColumnVector& angles);
}

// Global operators
ReturnMatrix operator* (const sml::Quaternion& q, const ColumnVector& v);

#endif /* UTIL_H_ */
