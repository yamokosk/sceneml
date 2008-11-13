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
 * stringutils.cpp
 *
 *  Created on: Nov 13, 2008
 *      Author: yamokosk
 */

#include <tinysg/stringutils.h>

#include <sstream>

namespace TinySG {

std::string toString( const ColumnVector& v)
{
	std::stringstream ss;
	for (unsigned int n=1; n < v.nrows(); ++n)
	{
		ss << v(n) << " ";
	}
	return ss.str();
}

std::string toString( const Quaternion& v)
{
	std::stringstream ss;
	ss << v.real() << " " << v.R_component_2() << " " << v.R_component_3() << " " << v.R_component_4();
	return ss.str();
}

}  // namespace TinySG
