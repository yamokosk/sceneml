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

math::Real MathExpression::getAsReal()
{
	return parseValue( expr_.c_str() );
}

int MathExpression::getAsInt()
{
	return (int)parseValue( expr_.c_str() );
}

ReturnMatrix MathExpression::getAsVector(unsigned int length)
{
	return parseVector( expr_.c_str() );
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
	parser_.SetExpr(str);
	// Evaluate string
	return (math::Real)parser_.Eval();
}

ReturnMatrix MathExpression::parseVector(const char* str)
{
	// Copy incoming char buffer to local one
	int length = strlen(str);
	char* buf = new char[length+1];

	memset(buf, '\0', (length+1)*sizeof(char));		
	memcpy(buf, str, length*sizeof(char));
	
	std::vector<math::Real> values;

	// Step through each token, evaluate it, and store it in our return vector
	char* tok = strtok(buf, " ,");
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

	// Delete temporary char buffer
	delete [] buf;

	// Copy answer into a Newmat column vector
	ColumnVector ret( values.size() );
	for (unsigned int n=0; n<values.size(); ++n) ret(n+1) = values[n];
	ret.Release();
	return ret;
}

}

}
