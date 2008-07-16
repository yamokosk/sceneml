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
 * PropertyPair.cpp
 *
 *  Created on: Jul 16, 2008
 *      Author: yamokosk
 */

#include "PropertyPair.h"
#include "math\smlMath.h"

namespace sml
{

PropertyPair::Parser.DefineConst("pi", pi);
PropertyPair::Parser.DefineConst("e", e);

std::string PropertyPair::getPropertyValue() const
{
	return value_;
}

sml::Real PropertyPair::getPropertyValueAsReal() const
{
	std::string expression = getPropertyValue();
	return PropertyPair::parseValue( expression.c_str() );
}

int PropertyPair::getPropertyValueAsInt() const
{
	std::string expression = getPropertyValue();
	return (int)PropertyPair::parseValue( expression.c_str() );
}

ReturnMatrix PropertyPair::getPropertyValueAsVector(unsigned int length) const
{
	std::string expression = getPropertyValue();
	return PropertyPair::parseVector( expression.c_str() );
}

/*ReturnMatrix PropertyPair::getPropertyValueAsMatrix(const std::string& name, unsigned int rows, unsigned int cols) const
{
	std::string expression = getPropertyValue(name);
	ColumnVector temp = PropertyPair::parseVector( expression.c_str() );
	return val;
}*/

static sml::Real PropertyPair::parseValue(const char* str)
{
	// Give expression to parser
	parser_.SetExpr(str);
	// Evaluate string
	return (sml::Real)parser_.Eval();
}

static ReturnMatrix PropertyPair::parseVector(const char* str)
{
	std::vector<sml::Real> values;

	// Step through each token, evaluate it, and store it in our return vector
	tok = strtok(str, " ,");
	while (tok != NULL)
	{
		try {
			sml::Real val = parseValue(tok);
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
