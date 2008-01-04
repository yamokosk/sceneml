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

#ifndef _SCENEML_ATTRIBUTES_H_FILE_
#define _SCENEML_ATTRIBUTES_H_FILE_

#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <stdexcept>

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI // Make sure M_PI is defined
#define M_PI 3.14159265358979323846
#endif

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

// Required math expression parser
#include <muParser.h>

namespace sceneml {

XERCES_CPP_NAMESPACE_USE

class Attributes
{
public:
	typedef std::map<std::string, std::string> properties_t;
	
	Attributes();
	virtual ~Attributes();
	
	void add(const std::string& name, const std::string &val);
	void update(const std::string& name, const std::string &val);
	void update(const char* name, const char* val);
	
	void get(const std::string& name, std::string& str);
	void get(const std::string& name, float* val);
	std::string operator()(const std::string& name) 
	{
		std::string str;
		this->get(name, str);
		return str;
	} 

private:
	void parseValue(const char* str, float* val);
	mu::Parser parser_;	
	properties_t properties_;
};

// Abstract Builder
class AttributesBuilder
{
protected:
	Attributes attrib_;
	std::string builderType_;
	
public:	
	AttributesBuilder(const DOMNode* node, const std::string& type) : node_(node), builderType_(type) {}
	virtual ~AttributesBuilder() {}
	Attributes GetAttributes() { return attrib_; }
 
	void createNewAttributes() { /*attrib_.erase();*/ }
 
	// General build methods
	void getAttributes();
	virtual void getParameters()=0;
	virtual void verify()=0;
	
	const DOMNode* node_;
};
 
// Director
class AttributesDirector
{
	AttributesBuilder* attribBuilder;
public:
	AttributesDirector() : attribBuilder(NULL) {}
	~AttributesDirector() {}
 
	void SetAttributesBuilder(AttributesBuilder* b) { attribBuilder = b; }
	Attributes GetAttributes() {return attribBuilder->GetAttributes();}
	void ConstructAttributes()
	{
		attribBuilder->createNewAttributes();
		attribBuilder->getAttributes();
		attribBuilder->getParameters();
		attribBuilder->verify();
	}
};

// Space attributes
class SpaceAttributesBuilder : public AttributesBuilder
{
public:
	SpaceAttributesBuilder(const DOMNode* node);
	
	void getAttributes();	
	void getParameters();
	void verify();
};

// Body attributes
class BodyAttributesBuilder : public AttributesBuilder
{
public:
	BodyAttributesBuilder(const DOMNode* node);
	
	void getAttributes();	
	void getParameters();
	void verify();
};

// Geom attributes
class GeomAttributesBuilder : public AttributesBuilder
{
public:
	GeomAttributesBuilder(const DOMNode* node);
	
	void getAttributes();	
	void getParameters();
	void verify();
};

// Translation attributes
class TranslationAttributesBuilder : public AttributesBuilder
{
public:
	TranslationAttributesBuilder(const DOMNode* node);
	
	void getAttributes();	
	void getParameters();
	void verify();
};

// Rotation attributes
class RotationAttributesBuilder : public AttributesBuilder
{
public:
	RotationAttributesBuilder(const DOMNode* node);
	
	void getAttributes();	
	void getParameters();
	void verify();
};

// Pair attributes
class PairAttributesBuilder : public AttributesBuilder
{
public:
	PairAttributesBuilder(const DOMNode* node);
	
	void getAttributes();	
	void getParameters();
	void verify();
};

// Rotation attributes
class MarkerAttributesBuilder : public AttributesBuilder
{
public:
	MarkerAttributesBuilder(const DOMNode* node);
	
	void getAttributes();	
	void getParameters();
	void verify();
};

// Transform attributes
class TransformAttributesBuilder : public AttributesBuilder
{
public:
	TransformAttributesBuilder(const DOMNode* node);
	
	void getAttributes();	
	void getParameters();
	void verify();
};

}; // END NAMESPACE

#endif
