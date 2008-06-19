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

#ifndef _SML_PROPERTY_BUILDER_H_FILE_
#define _SML_PROPERTY_BUILDER_H_FILE_

namespace sml {

// Abstract Builder
class PropertiesBuilder
{
protected:
	PropertyCollectionPtr pc_;
	std::string builderType_;
	
public:	
	PropertiesBuilder(const TiXmlElement* element, const std::string& type) : node_(element), builderType_(type), attrib_(new PropertyCollection) {};
	virtual ~PropertiesBuilder() {}
	
	PropertyCollectionPtr getProperties() { return pc_; }
 
	// General build methods
	virtual void createNewProperties() {pc_.reset(new PropertyCollection);}
	virtual void getAttributes();
	virtual void getParameters();
	virtual void verify();
	
	const TiXmlElement* node_;
};
 
// Director
class PropertiesDirector
{
	PropertiesBuilder* builder_;
public:
	PropertiesDirector() : builder_(NULL) {}
	~PropertiesDirector() {}
 
	void setBuilder(PropertiesBuilder* b) { builder_ = b; }
	void setBuilderAndBuild(PropertiesBuilder* b) { builder_ = b; constructProperties(); }
	PropertyCollectionPtr getProperties() {return builder_->getProperties();}
	void constructProperties()
	{
		builder_->createNewProperties();
		builder_->getAttributes();
		builder_->getParameters();
		builder_->verify();
	}
};

// Space attributes
class SpaceProperties : public PropertiesBuilder
{
public:
	SpaceProperties(const DOMNode* node);
	
	void createNewProperties();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Body attributes
class BodyProperties : public PropertiesBuilder
{
public:
	BodyProperties(const DOMNode* node);
	
	void createNewProperties();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Geom attributes
class GeomProperties : public PropertiesBuilder
{
public:
	GeomProperties(const DOMNode* node);

	void createNewProperties();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Translation attributes
class TranslationProperties : public PropertiesBuilder
{
public:
	TranslationProperties(const DOMNode* node);
	
	void createNewProperties();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Rotation attributes
class RotationProperties : public PropertiesBuilder
{
public:
	RotationProperties(const DOMNode* node);
	
	void createNewProperties();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Pair attributes
class PairProperties : public PropertiesBuilder
{
public:
	PairProperties(const DOMNode* node);
	
	void createNewProperties();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Rotation attributes
class MarkerProperties : public PropertiesBuilder
{
public:
	MarkerProperties(const DOMNode* node);
	
	void createNewProperties();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Transform attributes
class TransformProperties : public PropertiesBuilder
{
public:
	TransformProperties(const DOMNode* node);
	
	void createNewProperties();
	void getAttributes();	
	void getParameters();
	void verify();
};

}; // END NAMESPACE

#endif
