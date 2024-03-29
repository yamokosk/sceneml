/*************************************************************************
 * TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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
#include <memory>

// Internal
#include <tinysg/Map.h>
#include <tinysg/PropertyPair.h>
#include <tinysg/Exception.h>

#if defined(_WIN32)
#pragma warning(disable: 4290)
#endif

namespace TinySG {

class PropertyCollection
{
public:
	typedef MAP<std::string, PropertyPair> PairMap;
	typedef PairMap::const_iterator PropertyIterator;

	PropertyCollection();
	PropertyCollection(const PropertyCollection& pc);
	~PropertyCollection() {}

	PropertyCollection& operator= (const PropertyCollection& other);

	void addPair(const PropertyPair& pair) throw (TinySG::Exception);
	void updatePair(const char* key, const char* value, bool isRequired) throw (TinySG::Exception);
	const PropertyPair& getPair(size_t index) const throw (TinySG::Exception);

	unsigned int size() const {return (unsigned int)pairs_.size();}
	std::string getValue(const char* key) const throw (TinySG::Exception);
	bool hasProperty(const char* key) const;

	// Combine another set of properties with this one
	void combine(const PropertyCollection& pc);
private:
	PairMap pairs_;
};

typedef std::auto_ptr<PropertyCollection> PropertyCollectionPtr;

} // Namespace

#endif
