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
 
#ifndef _SML_MATH_H_FILE_
#define _SML_MATH_H_FILE_

#include <smlConfig.h>

#ifdef USE_DOUBLE_PRECISION
#include <math/smlDoubleConstants.h>
#else
#include <math/smlFloatConstants.h>
#endif

#include <cmath>
#include <boost/math/complex/asin.hpp>
#include <boost/math/complex/acos.hpp>
#include <boost/math/complex/atan.hpp>

namespace sml {

namespace math {

#ifdef USE_DOUBLE_PRECISION
typedef double Real;
#else
typedef float Real;
#endif

} // namespace sml::math

} // namespace sml

#endif