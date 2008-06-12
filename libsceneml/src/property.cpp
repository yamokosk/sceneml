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
 
#include "property.h"
#include <assert.h>
#include <sstream>
#include <string>
#include <fstream>

namespace sml {

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