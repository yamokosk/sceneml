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
 * PropertyPair.h
 *
 *  Created on: Jul 16, 2008
 *      Author: yamokosk
 */

#ifndef PROPERTYPAIR_H
#define PROPERTYPAIR_H

#include <math/Math.h>
#include <newmat/newmat.h>


namespace TinySG {

struct PropertyPair
{
	PropertyPair() : propertyName_("NO_DATA"), value_("NO_DATA"), required_(false) {}
	PropertyPair(const char* prop, const char* value="NO_DATA", bool isRequired="false") :
		propertyName_(prop), value_(value), required_(isRequired) {}
	virtual ~PropertyPair() {}

	// Get the property name
	std::string getPropertyName() const {return propertyName_;}

	// Various accessors for the value..
	std::string getPropertyValue() const {return value_;}
	bool isRequired() {return required_;}

private:
	std::string propertyName_;
	std::string value_;
	bool required_;
};

struct RequiredProperty : public PropertyPair
{
	RequiredProperty(const char* prop, const char* value="NO_DATA") :
		PropertyPair(prop, value, true) {}
};

struct OptionalProperty : public PropertyPair
{
	OptionalProperty(const char* prop, const char* value="NO_DATA") :
		PropertyPair(prop, value, false) {}
};


}

#endif
