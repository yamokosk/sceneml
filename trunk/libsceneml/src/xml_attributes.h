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

#include "config.h"

#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <stdexcept>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

// Boost shared ptr
#include <boost/shared_ptr.hpp>

namespace sml {

typedef boost::shared_ptr<dReal> dRealPtr;

XERCES_CPP_NAMESPACE_USE

typedef std::auto_ptr<Attributes> AttributesPtr;

// Abstract Builder
class AttributesBuilder
{
protected:
	AttributesPtr attrib_;
	std::string builderType_;
	
public:	
	AttributesBuilder(const DOMNode* node, const std::string& type) : node_(node), builderType_(type), attrib_(new Attributes) {};
	virtual ~AttributesBuilder() {}
	AttributesPtr GetAttributes() { return attrib_; }
 
	// General build methods
	virtual void createNewAttributes() {attrib_.reset(new Attributes);}
	virtual void getAttributes();
	virtual void getParameters()=0;
	virtual void verify()=0;
	
	const DOMNode* node_;
};
 
// Director
class SCENEML_API AttributesDirector
{
	AttributesBuilder* attribBuilder;
public:
	AttributesDirector() : attribBuilder(NULL) {}
	~AttributesDirector() {}
 
	void SetAttributesBuilder(AttributesBuilder* b) { attribBuilder = b; }
	AttributesPtr GetAttributes() {return attribBuilder->GetAttributes();}
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
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Body attributes
class BodyAttributesBuilder : public AttributesBuilder
{
public:
	BodyAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Geom attributes
class GeomAttributesBuilder : public AttributesBuilder
{
public:
	GeomAttributesBuilder(const DOMNode* node);

	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Translation attributes
class TranslationAttributesBuilder : public AttributesBuilder
{
public:
	TranslationAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Rotation attributes
class RotationAttributesBuilder : public AttributesBuilder
{
public:
	RotationAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Pair attributes
class PairAttributesBuilder : public AttributesBuilder
{
public:
	PairAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Rotation attributes
class MarkerAttributesBuilder : public AttributesBuilder
{
public:
	MarkerAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Transform attributes
class TransformAttributesBuilder : public AttributesBuilder
{
public:
	TransformAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

}; // END NAMESPACE

#endif
