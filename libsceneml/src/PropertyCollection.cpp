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

#include "PropertyCollection.h"
#include <assert.h>
#include <sstream>
#include <string>
#include <fstream>

namespace sml {

/**
 * Returns the property pair with specified index.
 */
PropertyPair PropertyCollection::getPair(size_t index) const throw (sml::Exception)
{
   PropertyIterator it = pairs_.begin();
   if (index >= pairs_.size())
   {
      std::ostringstream errTxt;
      errTxt << "Invalid property pair index" << (unsigned int)index;
      SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, errTxt.str());
   }

   for (size_t i=0; i<index; i++) it++;
   return it->second;
}

/**
 * Adds a new pair to the current contents.
 */
void PropertyCollection::addPair(const PropertyPair& pair) throw (sml::Exception)
{
	if (pairs_.find( pair.getPropertyName() ) != pairs_.end())
	{
		std::ostringstream msg;
		msg << "A property with the name " << pair.getPropertyName() << " already exists.";
		SML_EXCEPT(Exception::ERR_DUPLICATE_ITEM, msg.str());
	}

	pairs_[pair.getPropertyName()] = pair;
}

void PropertyCollection::updatePair(const char* key, const char* value, bool isRequired) throw (sml::Exception)
{
	std::map<std::string, PropertyPair>::iterator it = pairs_.find(key);

	if (it != pairs_.end()) {
		pairs_.erase(it);
		PropertyPair pair(key,value, isRequired);
		pairs_[key] = pair;
	} else {
		std::ostringstream msg;
		msg << "Property '" << key << "' not found in collection.";
		SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, msg.str());
	};
}

/**
 * Get value of the specified key (property).
 */
std::string PropertyCollection::getValue(const char* key) const throw (sml::Exception)
{
	PropertyIterator it;
	it = pairs_.find(key);
	if (it != pairs_.end())
		return it->second.getPropertyValue();

	std::ostringstream errTxt;
	errTxt << "Property '" << key << "' not found in collection.";
	SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, errTxt.str());
}


} // Namespace
