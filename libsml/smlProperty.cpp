/*************************************************************************
 * SML, Copyright (C) 2008  J.D. Yamokoski
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
 
#include "smlProperty.h"

#include <assert.h>
#include <sstream>
#include <string>
#include <fstream>

namespace sml {

PropertyPair::Parser.DefineConst("pi", pi);
PropertyPair::Parser.DefineConst("e", e);

std::string PropertyPair::getPropertyValue(const std::string& name) const
{
	return value_;
}

sml::Real PropertyPair::getPropertyValueAsReal(const std::string& name) const
{
	std::string expression = getPropertyValue(name);
	return PropertyPair::parseValue( expression.c_str() );
}
	
int PropertyPair::getPropertyValueAsInt(const std::string& name) const
{
	std::string expression = getPropertyValue(name);
	return (int)PropertyPair::parseValue( expression.c_str() );
}

ReturnMatrix PropertyPair::getPropertyValueAsVector(const std::string& name) const
{
	std::string expression = getPropertyValue(name);
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


/**
 * Returns the property pair with specified index.
 */
PropertyPair PropertyCollection::getPair(size_t index) const throw (SMLError)
{
   PropertyIterator it = pairs_.begin();
   if (index >= pairs_.size())
   {
      std::ostringstream errTxt;
      errTxt << (unsigned int)index << " - invalid property pair index";
      throw SMLError(errTxt.str().c_str(), MMERR_DEVICE_GENERIC);
   }

   for (size_t i=0; i<index; i++) it++;
   return it->second;
}

/**
 * Adds a new pair to the current contents.
 */
void PropertyCollection::addPair(const PropertyPair& pair)
{
	pairs_[pair.getPropertyName()] = pair;
}

void updatePair(const char* key, const char* value)
{
	std::map<std::string, PropertyPair>::iterator it = pairs_.find(name);
	
	if (it != properties_.end()) {
		pairs_.erase(it);
		PropertyPair pair(name,value);
		pairs_[name] = pair;
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "() - Parameter " + name + " not found in properties.";
		throw std::runtime_error(msg.str());
	};
}

/**
 * Get value of the specified key (property).
 */
std::string PropertyCollection::getValue(const char* key) const throw (SMLError)
{
	PropertyIterator it;
	it = pairs_.find(key);
	if (it != pairs_.end())
		return it->second.getPropertyValue();

	std::ostringstream errTxt;
	errTxt << key << " - invalid property name";
	throw SMLError(errTxt.str().c_str(), MMERR_DEVICE_GENERIC);
}

} // Namespace