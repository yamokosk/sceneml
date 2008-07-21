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
 * MathExpression.h
 *
 *  Created on: Jul 17, 2008
 *      Author: yamokosk
 */

#ifndef MATHEXPRESSION_H_
#define MATHEXPRESSION_H_

#include <string>
#include <math/smlMath.h>
#include <muParser.h>
#include <newmat/newmat.h>

namespace sml
{

namespace math
{

class MathExpression
{
public:
	MathExpression(const std::string& expr);
	virtual ~MathExpression();

	math::Real getAsReal();
	int getAsInt();
	ReturnMatrix getAsVector(unsigned int length);
	//ReturnMatrix getValAsMatrix(const std::string& name, unsigned int rows, unsigned int cols) const;

private:
	std::string expr_;
	mu::Parser parser_;

	math::Real parseValue(const char* str);
	ReturnMatrix parseVector(const char* str);
};

}

}

#endif /* MATHEXPRESSION_H_ */
