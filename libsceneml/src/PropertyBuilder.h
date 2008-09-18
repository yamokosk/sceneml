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
 * PropertyBuilder.h
 *
 *  Created on: Sep 17, 2008
 *      Author: yamokosk
 */

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
