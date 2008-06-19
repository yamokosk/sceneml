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

#include "xml_attributes.h"

namespace sml {

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
						<< "Parameter \"" << name.Value() << "=" << value.Value << "\" is illegal. Ignoring it." << std::endl;						
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

/**************************************************************************
 *	Space Attributes Builder
 **************************************************************************/
SpaceProperties::SpaceProperties(const TiXmlNode* node) : 
	PropertiesBuilder(node, "SpaceProperties") 
{ }

void SpaceProperties::createNewAttributes()
{
	PropertiesBuilder::createNewAttributes();
}

void SpaceProperties::getAttributes()
{
	pc_->addPair( RequiredProperty("name") );
	pc_->addPair( OptionalProperty("parent", "world") );
	//pc_->addPair( OptionalProperty("type", "simple") ); // This is specific to ODE!.. move it to a subclass
	
	// Parent class will get specific values
	PropertiesBuilder::getAttributes();
}

void SpaceProperties::getParameters()
{
	// The rest of the parameters depend on the type of space
	// ALL SPECIFIC TO ODE.. move to subclass
	//	simple:	No parameters
	//	hash:	center,	dVector3
	//			extents,	dVector3
	//			depth,	int
	//	quatree:	minlevel,	int
	//				maxlevel,	int
	/*std::string type = pc_->getValue("type");
	
	if (!type.compare("simple")) return; // No parameters for simple space
	else if (!type.compare("hash")) {
		pc_->addPair( PropertyPair("center", "0 0 0") );
		pc_->addPair( PropertyPair("extents", "0 0 0") );
		pc_->addPair( PropertyPair("depth", "4") );
	} else if (!type.compare("quadtree")) {
		pc_->addPair( PropertyPair("minlevel", "-1" ) );
		pc_->addPair( PropertyPair("maxlevel", "8") );
	} else {
		return;
	}*/
	
	// Parent class will get specific values
	PropertiesBuilder::getParameters();
}

void SpaceProperties::verify()
{
	// Parent class will check that required properties have a value other than NO_DATA
	PropertiesBuilder::verify()
	
	// Do specific verification steps here.. e.g. a property has one of three allowed values
}

/**************************************************************************
 *	Body Attributes Builder
 **************************************************************************/
BodyProperties::BodyProperties(const TiXmlNode* node) : 
	PropertiesBuilder(node, "BodyProperties") 
{
	
}

void BodyProperties::createNewAttributes()
{
	PropertiesBuilder::createNewAttributes();
}

void BodyProperties::getAttributes()
{
	// Tag specific attributes and default values
	pc_->addPair( RequiredProperty("name") );
	pc_->addPair( OptionalProperty("parent", "world") );
	
	// Parent class will get specific values
	PropertiesBuilder::getAttributes();
}

void BodyProperties::getParameters()
{
	// Bodies currently have no parameters
	
	// Parent class will get specific values
	PropertiesBuilder::getParameters();
}

void BodyProperties::verify()
{
	// Parent class will check that required properties have a value other than NO_DATA
	PropertiesBuilder::verify()
	
	// Do specific verification steps here.. e.g. a property has one of three allowed values
}


/**************************************************************************
 *	Geom Attributes Builder
 **************************************************************************/
GeomProperties::GeomProperties(const TiXmlNode* node) : 
	PropertiesBuilder(node, "GeomProperties") 
{
	
}

void GeomProperties::createNewAttributes()
{
	PropertiesBuilder::createNewAttributes();
}

void GeomProperties::getAttributes()
{
	// Tag specific attributes and default values
	pc_->addPair( RequiredProperty("name") );
	pc_->addPair( RequiredProperty("type") );
	pc_->addPair( OptionalProperty("parent", "world") );
		
	// Parent class will get specific values
	PropertiesBuilder::getAttributes();
}

void GeomProperties::getParameters()
{
	// Tag specific parameters and default values
	pc_->addPair( OptionalProperty("color", "1.0 1.0 1.0") );
	pc_->addPair( OptionalProperty("scale", "1.0") );
	pc_->addPair( OptionalProperty("alpha", "1.0") );
	pc_->addPair( OptionalProperty("checkcollision", "1.0") );
	
	// The rest of the parameters depend on the type of geom
	//	box:			length, width, height (all doubles)
	//	ccylinder:	radius, length (all doubles)
	//	sphere:		radius (all doubles)
	//	plane:		normal_x, normal_y, normal_z, d (all doubles)
	//	mesh:		filname (std::string)	
	std::string type = pc_->getValue("type");
	
	if (!type.compare("box")) {
		pc_->addPair( RequiredProperty("length") );
		pc_->addPair( RequiredProperty("width") );
		pc_->addPair( RequiredProperty("height") );
	} else if (!type.compare("ccylinder")) {
		pc_->addPair( RequiredProperty("radius") );
		pc_->addPair( RequiredProperty("length") );
	} else if (!type.compare("cylinder")) {
		pc_->addPair( RequiredProperty("radius") );
		pc_->addPair( RequiredProperty("length") );
	} else if (!type.compare("sphere")) {
		pc_->addPair( RequiredProperty("radius") );
	} else if (!type.compare("plane")) {
		pc_->addPair( RequiredProperty("normal_x") );
		pc_->addPair( RequiredProperty("normal_y") );
		pc_->addPair( RequiredProperty("normal_z") );
		pc_->addPair( RequiredProperty("d") );
	} else if (!type.compare("mesh")) {
		pc_->addPair( RequiredProperty("filename") );
	} else {
		return;
	}
	
	// Parent class will get specific values
	PropertiesBuilder::getParameters();
}

void GeomProperties::verify()
{
	// Parent class will check that required properties have a value other than NO_DATA
	PropertiesBuilder::verify()
	
	// Do specific verification steps here.. e.g. a property has one of three allowed values
}

/**************************************************************************
 *	Translation Attributes Builder
 **************************************************************************/
TranslationProperties::TranslationProperties(const TiXmlNode* node) : 
	PropertiesBuilder(node, "TranslationProperties") 
{

}

void TranslationProperties::createNewAttributes()
{
	PropertiesBuilder::createNewAttributes();
}

void TranslationProperties::getAttributes()
{
	// Tag specific attributes and default values
	pc_->addPair( OptionalProperty("type") ); // Type is not defined for translations.. but its here for consistency
	pc_->addPair( OptionalProperty("value", "0 0 0") );
	pc_->addPair( OptionalProperty("mutable", "0") );
	pc_->addPair( OptionalProperty("name") );
	
	// Parent class will get specific values
	PropertiesBuilder::getAttributes();
}

void TranslationProperties::getParameters()
{
	// No parameters
	
	// Parent class will get specific values
	PropertiesBuilder::getParameters();
}

void TranslationProperties::verify()
{
	// Parent class will check that required properties have a value other than NO_DATA
	PropertiesBuilder::verify()
	
	// Do specific verification steps here.. e.g. a property has one of three allowed values
	std::string isMutable = pc_->getValue("mutable");
	
	if (!isMutable.compare("true")) {
		std::string name = pc_->getValue("name");
		
		if (!name.compare("NO_DATA"))
			throw std::runtime_error(builderType_ + "::verify() - A translation is marked mutable but has no name!");
	}
}

/**************************************************************************
 *	Rotation Attributes Builder
 **************************************************************************/
RotationProperties::RotationProperties(const TiXmlNode* node) : 
	PropertiesBuilder(node, "RotationProperties") 
{

}

void RotationProperties::createNewAttributes()
{
	PropertiesBuilder::createNewAttributes();
}

void RotationProperties::getAttributes()
{
	// Tag specific attributes and default values
	pc_->addPair( RequiredProperty("type") );
	pc_->addPair( OptionalProperty("value", "0") );
	pc_->addPair( OptionalProperty("mutable", "0") );
	pc_->addPair( OptionalProperty("name") );
	
	// Parent class will get specific values
	PropertiesBuilder::getAttributes();
}

void RotationProperties::getParameters()
{
	// No parameters
	
	// Parent class will get specific values
	PropertiesBuilder::getParameters();
}

void RotationProperties::verify()
{
	// Parent class will check that required properties have a value other than NO_DATA
	PropertiesBuilder::verify()
	
	// Do specific verification steps here.. e.g. a property has one of three allowed values
	std::string isMutable = pc_->getValue("mutable");
	
	if (!isMutable.compare("true")) {
		std::string name = pc_->getValue("name");
		
		if (!name.compare("NO_DATA"))
			throw std::runtime_error(builderType_ + "::verify() - A rotation is marked mutable but has no name!");
	}
}

/**************************************************************************
 *	Pair Attributes Builder
 **************************************************************************/
PairProperties::PairProperties(const TiXmlNode* node) : 
	PropertiesBuilder(node, "PairProperties") 
{

}

void PairProperties::createNewAttributes()
{
	PropertiesBuilder::createNewAttributes();
}

void PairProperties::getAttributes()
{
	// Tag specific attributes and default values
	pc_->addPair( RequiredProperty("space1") );
	pc_->addPair( RequiredProperty("space2") );
	
	// Parent class will get specific values
	PropertiesBuilder::getAttributes();
}

void PairProperties::getParameters()
{
	// No parameters
	
	// Parent class will get specific values
	PropertiesBuilder::getParameters();
}

void PairProperties::verify()
{
	// Parent class will check that required properties have a value other than NO_DATA
	PropertiesBuilder::verify()
	
	// Do specific verification steps here.. e.g. a property has one of three allowed values
}


/**************************************************************************
 *	Marker Attributes Builder
 **************************************************************************/
MarkerProperties::MarkerProperties(const TiXmlNode* node) : 
	PropertiesBuilder(node, "MarkerProperties") 
{
}

void MarkerProperties::createNewAttributes()
{
	PropertiesBuilder::createNewAttributes();
}

void MarkerProperties::getAttributes()
{
	// Tag specific attributes and default values
	pc_->addPair( RequiredProperty("type") );
	pc_->addPair( OptionalProperty("value", "0 0 0") );
	pc_->addPair( OptionalProperty("mutable", "0") );
	pc_->addPair( OptionalProperty("name") );
	
	// Parent class will get specific values
	PropertiesBuilder::getAttributes();
}

void MarkerProperties::getParameters()
{
	// No parameters
	
	// Parent class will get specific values
	PropertiesBuilder::getParameters();
}

void MarkerProperties::verify()
{
	// Parent class will check that required properties have a value other than NO_DATA
	PropertiesBuilder::verify()
	
	// Do specific verification steps here.. e.g. a property has one of three allowed values
	std::string isMutable = pc_->getValue("mutable");
	
	if (!isMutable.compare("true")) {
		std::string name = pc_->getValue("name");
		
		if (!name.compare("NO_DATA"))
			throw std::runtime_error(builderType_ + "::verify() - A marker is marked mutable but has no name!");
	}
}


/**************************************************************************
 *	Transform Attributes Builder
 **************************************************************************/
TransformProperties::TransformProperties(const TiXmlNode* node) : 
	PropertiesBuilder(node, "TransformProperties") 
{

}

void TransformProperties::createNewAttributes()
{
	PropertiesBuilder::createNewAttributes();
}

void TransformProperties::getAttributes()
{
	// Tag specific attributes and default values
	pc_->addPair( OptionalProperty("type", "simple") );
	
	// Parent class will get specific values
	PropertiesBuilder::getAttributes();
}

void TransformProperties::getParameters()
{
	// No parameters
	
	// Parent class will get specific values
	PropertiesBuilder::getParameters();
}

void TransformProperties::verify()
{
	// Parent class will check that required properties have a value other than NO_DATA
	PropertiesBuilder::verify()
	
	// Do specific verification steps here.. e.g. a property has one of three allowed values
}


}; // END NAMESPACE
