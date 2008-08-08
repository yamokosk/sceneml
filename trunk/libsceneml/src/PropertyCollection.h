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

#ifndef PROPERTYCOLLECTION_H
#define PROPERTYCOLLECTION_H

#include <string>
#include <hash_map>
#include "PropertyPair.h"
#include "Exception.h"

#if defined(_WIN32)
#pragma warning(disable: 4290)
#endif

namespace tinysg {

class PropertyCollection
{
public:
	typedef stdext::hash_map<std::string, PropertyPair> PairMap;
	typedef PairMap::const_iterator PropertyIterator;

	PropertyCollection() {}
	~PropertyCollection() {}

	void addPair(const PropertyPair& pair) throw (tinysg::Exception);
	void updatePair(const char* key, const char* value, bool isRequired) throw (tinysg::Exception);
	PropertyPair getPair(size_t index) const throw (tinysg::Exception);

	size_t size() const {return pairs_.size();}
	std::string getValue(const char* key) const throw (tinysg::Exception);

private:
	PairMap pairs_;
};

} // Namespace

#endif
