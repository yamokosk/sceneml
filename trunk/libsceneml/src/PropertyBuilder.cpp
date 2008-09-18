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
 * PropertyBuilder.cpp
 *
 *  Created on: Sep 17, 2008
 *      Author: yamokosk
 */


#include "PropertyBuilder.h"

namespace TinySG {

/**************************************************************************

 *	Base Attributes Builder

 **************************************************************************/
void PropertiesBuilder::getAttributes()
{
	TiXmlAttribute* attribute = node_->FirstAttribute();

	// Update (not add) all existing attributes against
	while ( attribute != NULL )
	{
		const std::string & 	ValueStr () const

		try {
			pc_->update( attribute.Name(), attribute.Value() );
		} catch (std::runtime_error &ex) {
			std::cerr << ex.what() << std::endl
					<< builderType_ << __FUNCTION__ << "(): "
					<< "In space \"" << pc_->getValue("name")
					<< "\", unrecognized attribute \"" << attrName << "=" << attrValue
					<< "\". Ignoring it." << std::endl;
		};

		// Get the next attribute
		attribute = attribute.Next();
	}
}

void PropertiesBuilder::getParameters()
{
	TiXmlNode* node = node_->FirstChild("parameter");
	while ( node != NULL )
	{
		// All parameter tags in sml should be formed as follows:
		//	<parameter name="_name_" value="_value_" />
		TiXmlAttribute* name = node_->FirstAttribute();
		TiXmlAttribute* value = name.Next();

		try {
			pc_->update(name.Value(), value.Value());
		} catch (std::runtime_error &ex) {
			std::cerr 	<< ex.what() << std::endl << "Line: " << name.Row()
						<< "Parameter \"" << name.Value() << "=" << value.Value() << "\" is illegal. Ignoring it." << std::endl;
		}

		// Get next sibiling
		node =  node->NextSibling("parameter");
	}
}

// Simply checks that any required properties have something entered other than "NO_DATA"
// More detailed verification is left up to the derived classes.
void PropertiesBuilder::verify()
{
	unsigned int numProperties = pc_->size();
	for (unsigned int n=0; n < numProperties; ++n)
	{
		PropertyPair pair = pc_->getPair(n);
		std::string value = pair.getPropertyValue();
		if (pair.isRequired() && value.compare("NO_DATA"))
		{
			stringstream msg;
			msg << "Property \"" << pair.getName() << "\" was required but I did not find a value for it." << std::endl;
			throw std::runtime_error( msg.str() );
		}
	}
}
