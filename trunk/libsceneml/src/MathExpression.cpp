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
 * MathExpression.cpp
 *
 *  Created on: Jul 17, 2008
 *      Author: yamokosk
 */

#include "MathExpression.h"
#include <cstring>

namespace sml
{

namespace math
{

MathExpression::MathExpression(const std::string& expr) :
	expr_(expr)
{
	parser_.DefineConst("pi", math::pi);
	parser_.DefineConst("e", math::e);
}

MathExpression::~MathExpression()
{

}

math::Real MathExpression::getExprAsReal() const
{
	return PropertyPair::parseValue( expr_.c_str() );
}

int MathExpression::getExprAsInt() const
{
	return (int)PropertyPair::parseValue( expr_.c_str() );
}

ReturnMatrix MathExpression::getExprAsVector(unsigned int length) const
{
	return PropertyPair::parseVector( expr_.c_str() );
}

/*ReturnMatrix MathExpression::getPropertyValueAsMatrix(const std::string& name, unsigned int rows, unsigned int cols) const
{
	std::string expression = getPropertyValue(name);
	ColumnVector temp = PropertyPair::parseVector( expression.c_str() );
	return val;
}*/

math::Real MathExpression::parseValue(const char* str)
{
	// Give expression to parser
	PropertyPair::Parser.SetExpr(str);
	// Evaluate string
	return (math::Real)Parser.Eval();
}

ReturnMatrix MathExpression::parseVector(const char* str)
{
	std::vector<math::Real> values;

	// Step through each token, evaluate it, and store it in our return vector
	char* tok = strtok(str, " ,");
	while (tok != NULL)
	{
		try {
			math::Real val = parseValue(tok);
			values.push_back( val );
		} catch (mu::Parser::exception_type &e) {
			std::cout << e.GetMsg() << std::endl;
		}

		// Get new token
		tok = strtok(NULL, " ,");
	}

	ColumnVector ret( values.size() );
	for (unsigned int n=0; n<values.size(); ++n) ret(n+1) = values[n];
	ret.Release();
	return ret;
}

}

}
