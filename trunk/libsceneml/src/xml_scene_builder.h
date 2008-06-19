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

// TODO: Rename xml_scene_builder to ode_scene_builder or lc_scene_builder

#ifndef _SCENEML_XML_SCENE_BUILDER_H_FILE_
#define _SCENEML_XML_SCENE_BUILDER_H_FILE_

#include "config.h"
#include "sceneobjects.h"
#include "transform.h"
#include "Primitive.h"
#include "scene_builder.h"

// Required Xerces headers
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

namespace sceneml {

XERCES_CPP_NAMESPACE_USE

// ConcreteBuilder
class SCENEML_API XMLSceneBuilder : public SceneBuilder
{
public:
	XMLSceneBuilder(const char* filename);
	~XMLSceneBuilder();

	// Redefinition of parent functions
	void buildSpaces();
	void buildBodies();
	void buildGeoms();

private:	
	XMLSceneBuilder();
	
	// XML specific functions and data members
	void readXMLDescription( );
	void parseTransform(DOMNode *node, CompositeTransform *pTransform, Body *b=NULL);
	CoordinateTransformPtr parseSimpleTransform(DOMNode *node, Body *b=NULL);
	CoordinateTransformPtr parseMarkerTransform(DOMNode *node, Body *b=NULL);
	void isFileValid();
	
	DOMImplementation* impl_;
	DOMBuilder* parser_;
	DOMDocument* domDoc_;
	
	char filename_[256];
};

};

#endif
