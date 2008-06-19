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


#include "xml_scene_builder.h"
#include "xml_attributes.h"
#include "MeshImport.h"

#include <fstream>

namespace sceneml {

SceneBuilder::SceneBuilder(const char* filename) :
	doc_(filename),
	SceneBuilderBase()
{
	if ( !doc_.LoadFile() )
	{
		// Throw error
	} else {
		root_ = doc_.RootElement();
		
		if (root != NULL)
		{
			// Throw another error
		}
	}
}

SceneBuilder::~SceneBuilder()
{
}

void SceneBuilder::buildSpaces()
{
	TiXmlNode* node = root_->FirstChild ("space");
	
	// Loop over all space nodes
	while (node != NULL)
	{
		SpaceProperties builder(node);
		propertyDirector_.setBuilderAndBuild( &builder );
		PropertyCollectionPtr properties = propertyDirector_.getProperties();
		
		// Create a new space object
		scene_->createSpace( properties );
					
		// Get next space node
		node = node_->NextSibling("space");
	}
	
	TiXmlNode* node = root_->FirstChild ("pair");
	
	// Loop over all pair nodes
	while (node != NULL)
	{
		PairProperties builder(node);
		propertyDirector_.setBuilderAndBuild( &builder );
		PropertyCollectionPtr properties = propertyDirector_.getProperties();
		
		// Create a new space object
		scene_->addCollisionPair( properties->getValue("space1"), properties->getValue("space1") );
					
		// Get next space node
		node = node_->NextSibling("pair");
	}
}

void SceneBuilder::buildBodies()
{
	try {
		// With a valid DOMDocument, its time to populate the Scene structure.
		XMLCh tagname[30];
		XMLString::transcode("body", tagname, 29);
		DOMNodeList* allBodyItems = domDoc_->getElementsByTagName(tagname);

		// Loop over all of the bodies
		for (unsigned int bodyIndex = 0; bodyIndex < allBodyItems->getLength(); ++bodyIndex) 
		{
			DOMNode* thisBodyItem = allBodyItems->item(bodyIndex);
		
			AttributesDirector attribDirector;

			BodyAttributesBuilder bodyAttribBuilder(thisBodyItem);
			attribDirector.SetAttributesBuilder( &bodyAttribBuilder );
			attribDirector.ConstructAttributes();

			AttributesPtr attrib = attribDirector.GetAttributes();
			
			//dBodyID b = dBodyCreate(scene_->getWorld());
			Body* body = scene_->createBody( attrib );
			Body* prox = scene_->getBody(attrib->getValAsStr("parent"));
			body->setProxObj(prox);
			prox->addDistBody(body);
			
			CompositeTransformPtr pRootTransform( new CompositeTransform() );
			parseTransform(thisBodyItem, pRootTransform.get(), body);
			body->setCompositeTransform(pRootTransform);						

			body->invalidate();
		}
		
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const DOMException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const std::string& str) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << str;
		throw std::runtime_error(msg.str());
	} catch (const std::runtime_error &ex) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << ex.what();
		throw std::runtime_error(msg.str());
	} catch (...) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Unknown or unexpected error.";
		throw std::runtime_error(msg.str());
	}
	//std::cout << "Finished building bodies." << std::endl;
}

void SceneBuilder::buildGeoms()
{
	try {
		// With a valid DOMDocument, its time to populate the Scene structure.
		XMLCh tagname[30];
		XMLString::transcode("geometry", tagname, 29);
		DOMNodeList* allGeomItems = domDoc_->getElementsByTagName(tagname);
		
		// Loop over all of the spaces
		for (unsigned int geomIndex = 0; geomIndex < allGeomItems->getLength(); ++geomIndex) 
		{
			DOMNode* thisGeomItem = allGeomItems->item(geomIndex);
		
			AttributesDirector attribDirector;

			GeomAttributesBuilder geomAttribBuilder(thisGeomItem);
			attribDirector.SetAttributesBuilder( &geomAttribBuilder );
			attribDirector.ConstructAttributes();
			AttributesPtr geomAttrib = attribDirector.GetAttributes();
			
			// Get geom's body
			DOMNode* thisGeomBodyItem = thisGeomItem->getParentNode();
			AttributesDirector bodyDirector;
			BodyAttributesBuilder bodyAttribBuilder(thisGeomBodyItem);
			bodyDirector.SetAttributesBuilder( &bodyAttribBuilder );
			bodyDirector.ConstructAttributes();
			AttributesPtr bodyAttrib = bodyDirector.GetAttributes();
			Body* body = scene_->getBody(bodyAttrib->getValAsStr("name"));
			
			// Get geom's space
			DOMNode* thisGeomSpaceItem = thisGeomBodyItem->getParentNode();
			AttributesDirector spaceDirector;
			SpaceAttributesBuilder spaceAttribBuilder(thisGeomSpaceItem);
			spaceDirector.SetAttributesBuilder( &spaceAttribBuilder );
			spaceDirector.ConstructAttributes();
			AttributesPtr spaceAttrib = spaceDirector.GetAttributes();
			dSpaceID spaceID = scene_->getSpace(spaceAttrib->getValAsStr("name"));
			
			// Construct the ODE object - based on type of course
			Geom* geom = scene_->createGeom(geomAttrib);
			geom->setProxObj(body);
			geom->setCompositeTransform(pTransform);
			body->addGeom(geom);
			
			// Set other properties
			geom->setProperty( "Color", geomAttrib->getValAsStr("color") );
			geom->setProperty( "Alpha", geomAttrib->getValAsStr("alpha") );
			geom->setProperty( "CollisionCheck", geomAttrib->getValAsStr("checkcollision") );
						
			// Invalidate and move on to next geom
			geom->invalidate();
		}
		
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const DOMException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const std::string& str) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << str;
		throw std::runtime_error(msg.str());
	} catch (const std::runtime_error &ex) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << ex.what();
		throw std::runtime_error(msg.str());
	} catch (...) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Unknown or unexpected error.";
		throw std::runtime_error(msg.str());
	}
	//std::cout << "Finished building geoms." << std::endl;
}

//TransformList_t SceneBuilder::parseTransform(DOMNode *node)
void SceneBuilder::parseTransform(DOMNode *node, CompositeTransform* pRootTransform, Body *b)
{
	DOMNodeList *allChildItems = node->getChildNodes();
		
	for (unsigned int c=0; c < allChildItems->getLength(); ++c)
	{
		DOMNode *thisChildItem = allChildItems->item(c);
		
		if (thisChildItem->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			std::auto_ptr<char> pChildTagName( XMLString::transcode(thisChildItem->getNodeName()) );
						
			// Find the transform tag
			if (strcmp(pChildTagName.get(), "transform") == 0) 
			{
				TransformProperties builder(node);
				propertyDirector_.setBuilderAndBuild( &builder );
				PropertyCollectionPtr properties = propertyDirector_.getProperties();
		
				// Create a new space object
				scene_->createSpace( properties );
										
				std::string type = attrib->getValAsStr("type");
				CoordinateTransformPtr pTransform;
				
				if ( !type.compare("simple") ) pTransform = parseSimpleTransform(thisChildItem, b);
				else if ( !type.compare("marker") ) pTransform = parseMarkerTransform(thisChildItem, b);
				
				if (pTransform.get() != NULL) pRootTransform->add(pTransform);
			}
		}
	}
	
	return;
}

CoordinateTransformPtr SceneBuilder::parseSimpleTransform(DOMNode *node, Body *b) 
{
	//CompositeTransform* pSimpleTransform = new CompositeTransform();
	CoordinateTransformPtr pSimpleTransform( new CompositeTransform() );
	
	DOMNodeList *allTransformItems = node->getChildNodes();

	for (unsigned int t=0; t < allTransformItems->getLength(); ++t) 
	{
		DOMNode *thisTransformItem = allTransformItems->item(t);
		
		if (thisTransformItem->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			char* pTransformTagName = XMLString::transcode(thisTransformItem->getNodeName());

			// Parse the attributes.
			AttributesDirector attribDirector;
			AttributesPtr attrib; std::auto_ptr<AttributesBuilder> pBuilder;
			
			if ( strcmp(pTransformTagName, "translation") == 0 ) {
				pBuilder.reset( new TranslationAttributesBuilder(thisTransformItem) );
			} else if ( strcmp(pTransformTagName, "rotation") == 0 ) {
				pBuilder.reset( new RotationAttributesBuilder(thisTransformItem) );
			} else {
				std::ostringstream msg;
				msg << __FUNCTION__ << "(): Found an illegal tag " << pTransformTagName << " within a simple transformation.";
				XMLString::release(&pTransformTagName);
				throw std::runtime_error(msg.str());
			}
			attribDirector.SetAttributesBuilder( pBuilder.get() );
			attribDirector.ConstructAttributes();
			attrib = attribDirector.GetAttributes();
			
			// Create the transform object
			//SimpleTransform *aTransform = NULL;
			CoordinateTransformPtr aTransform;
			if ( strcmp(pTransformTagName, "translation") == 0 ) {
				aTransform.reset( new SimpleTransform("translation", "_NODATA_") );
			} else if ( strcmp(pTransformTagName, "rotation") == 0 ) {
				aTransform.reset( new SimpleTransform("rotation", attrib->getValAsStr("type")) );
			}
			XMLString::release(&pTransformTagName);

			// Add value
			dRealPtr value = attrib->getValAsVec("value", 3);
			((SimpleTransform*)aTransform.get())->setData(value);
			
			// If value is mutable, register it with the scene			
			float isMutable = attrib->getValAsReal("mutable");
			if ( isMutable ) {
				if (b == NULL) {
					std::ostringstream msg;
					msg << __FUNCTION__ << "(): Found a mutable variable for a geometry. This is not allowed!";
					throw std::runtime_error(msg.str());
				} else {
					scene_->addMutableValue(attrib->getValAsStr("name"), value, b);
				}
			}
			
			// Add simple transform to the composite
			((CompositeTransform*)pSimpleTransform.get())->add( aTransform );
		}   
	}
				
	//return transList;
	return pSimpleTransform;
}

CoordinateTransformPtr SceneBuilder::parseMarkerTransform(DOMNode *node, Body *b) 
{
	//MarkerTransform* pMarkerTransform = new MarkerTransform();
	CoordinateTransformPtr pMarkerTransform( new MarkerTransform() );
	int nLocalCoord = 0, nGlobalCoord = 0;
	
	DOMNodeList *allTransformItems = node->getChildNodes();

	for (unsigned int t=0; t < allTransformItems->getLength(); ++t) 
	{
		DOMNode *thisTransformItem = allTransformItems->item(t);
		
		if (thisTransformItem->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			char* pTransformTagName = XMLString::transcode(thisTransformItem->getNodeName());
			if ( strcmp(pTransformTagName, "marker") != 0 ) 
			{
				std::ostringstream msg;
				msg << __FUNCTION__ << "(): Tag " << pTransformTagName << " inside a marker transformation is illegal.";
				throw std::runtime_error(msg.str());
			}
			XMLString::release(&pTransformTagName);
			
			// Get marker attributes
			AttributesDirector attribDirector;
			MarkerAttributesBuilder markerAttribBuilder(thisTransformItem);
			attribDirector.SetAttributesBuilder( &markerAttribBuilder );
			attribDirector.ConstructAttributes();
			AttributesPtr attrib = attribDirector.GetAttributes();
			
			dRealPtr value = attrib->getValAsVec("value", 3);
			std::string type = attrib->getValAsStr("type");
			if ( !type.compare("local") ){
				((MarkerTransform*)pMarkerTransform.get())->addLocalCoord( value );
				nLocalCoord++;
			} else if ( !type.compare("global") ) {
				((MarkerTransform*)pMarkerTransform.get())->addGlobalCoord( value );
				nGlobalCoord++;
			} else {
				std::ostringstream msg;
				msg << __FUNCTION__ << "(): Marker type " << type << " is illegal. It can either be local or global.";
				throw std::runtime_error(msg.str());
			}

			// If value is mutable, register it with the scene			
			float isMutable = attrib->getValAsReal("mutable");
			if ( isMutable ) {
				if (b == NULL) {
					std::ostringstream msg;
					msg << __FUNCTION__ << "(): Found a mutable variable for a geometry. This is not allowed!";
					throw std::runtime_error(msg.str());
				} else {
					scene_->addMutableValue(attrib->getValAsStr("name"), value, b);
				}
			}
		}   
	}
	
	if (nLocalCoord != nGlobalCoord) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Found " << nLocalCoord << " local and " << nGlobalCoord
		    << " global coordinates. These must be equal!";
		throw std::runtime_error(msg.str());
	}
		
	return pMarkerTransform;
}

}; // NAMESPACE
