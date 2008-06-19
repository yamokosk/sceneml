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

 #ifndef _SML_PROPERTY_H_FILE_
#define _SML_PROPERTY_H_FILE_

#include <string>
#include <map>
#include <memory>

// Require newmat library
#include <newmat.h>

// Required math expression parser
#include <muParser.h>

namespace sml {

struct PropertyPair
{
	PropertyPair(const char* prop, const char* value="NO_DATA", bool isRequired) :
		propertyName_(prop), value_(value), required_(isRequired) {}
	virtual ~PropertyPair() {}
	
	// Get the property name
	std::string getPropertyName() const {return propertyName_;}

	// Various accessors for the value..
	std::string getPropertyValue() const {return value_;}
	sml::Real getPropertyValueAsReal(const std::string& name) const;
	int getPropertyValueAsInt(const std::string& name) const;	
	ReturnMatrix getValAsVector(const std::string& name, unsigned int length) const;
	//ReturnMatrix getValAsMatrix(const std::string& name, unsigned int rows, unsigned int cols) const;
	
	bool isRequired() {return required_;}
	
private:
	std::string propertyName_;
	std::string value_;
	bool required_;
	
	static mu::Parser Parser;
	
	static sml::Real parseValue(const char* str);
	static ReturnMatrix parseVector(const char* str);
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

class PropertyCollection
{
public:
	typedef std::map<std::string, PropertyPair>::const_iterator PropertyIterator;
	
	PropertyCollection() {}
	~PropertyCollection() {}

	void addPair(const PropertyPair& pair);
	void updatePair(const char* key, const char* value);
	PropertyPair getPair(size_t index) const throw (SMLError);
   
	size_t size() const {return pairs_.size();}
	std::string getValue(const char* key) const throw (SMLError);
 
private:
	std::map<std::string, PropertyPair> pairs_;
};

typedef std::auto_ptr<PropertyCollection> PropertyCollectionPtr;

} // Namespace