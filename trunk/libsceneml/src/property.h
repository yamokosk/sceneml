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
 
#include <string>
#include <map>

namespace sml {

struct PropertyPair
{
	PropertyPair(const char* prop, const char* value) :
		propertyName_(prop), value_(value) {}
	~PropertyPair() {}
	
	std::string getPropertyName() const {return propertyName_;}
	std::string getPropertyValue() const {return value_;}

private:
	std::string propertyName_;
	std::string value_;
};

class PropertyCollection
{
public:
	typedef std::map<std::string, PropertyPair>::const_iterator PropertyIterator;
	
	PropertyCollection() {}
	~PropertyCollection() {}

	void addPair(const PropertyPair& pair);
	PropertyPair getPair(size_t index) const throw (SMLError);
   
	size_t size() const {return pairs_.size();}
	std::string getValue(const char* key) const throw (SMLError);
 
private:
	std::map<std::string, PropertyPair> pairs_;
};

} // Namespace