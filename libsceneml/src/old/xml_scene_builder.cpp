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


#include "xml_scene_builder.h"
#include "xml_attributes.h"
#include "MeshImport.h"

#include <fstream>

namespace sceneml {

XMLSceneBuilder::XMLSceneBuilder() :
	SceneBuilder()
{
	try {
		XMLPlatformUtils::Initialize();
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): XML platform initialization error. " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	}
	
	memset(filename_, '\0', sizeof(filename_));
}

XMLSceneBuilder::XMLSceneBuilder(const char* filename) :
	SceneBuilder()
{
	memset(filename_, '\0', sizeof(filename_));
	memcpy(filename_, filename, sizeof(char)*strlen(filename));

	try {
		XMLPlatformUtils::Initialize();
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): XML platform initialization error. " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	}
	
	this->isFileValid();
	this->readXMLDescription();
}

XMLSceneBuilder::~XMLSceneBuilder()
{
	parser_->release();
	XMLPlatformUtils::Terminate();
}

void XMLSceneBuilder::isFileValid()
{
	std::fstream fp(filename_, std::fstream::in);  // declarations of streams fp_in and fp_out
	if (!fp.good()) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << filename_ << " is not a valid XML file.";
		throw std::runtime_error(msg.str());
	}
	fp.close();
}

void XMLSceneBuilder::readXMLDescription( )
{
	// Initialize DOM parser and set properties
	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99); // What is LS? Need comment here!  -J.D.
	impl_ = DOMImplementationRegistry::getDOMImplementation(tempStr);
	parser_ = ((DOMImplementationLS*)impl_)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

	// FUTURE WORK - Lookup and use features of the DOM parser.. sure there is
	// something useful I could use in there.  -J.D.
	//if (parser->canSetFeature(XMLUni::fgDOMValidation, true))
	//	parser->setFeature(XMLUni::fgDOMValidation, true);
	//if (parser->canSetFeature(XMLUni::fgDOMNamespaces, true))
	//	parser->setFeature(XMLUni::fgDOMNamespaces, true);
	//if (parser->canSetFeature((XMLUni::fgDOMDatatypeNormalization, true))
	//	parser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);


	// optionally you can implement your DOMErrorHandler (e.g. MyDOMErrorHandler)
	// and set it to the builder
	//MyDOMErrorHandler* errHandler = new myDOMErrorHandler();
	//parser->setErrorHandler(errHandler);
	
	// Now parse the actual document
	try {
		domDoc_ = parser_->parseURI( filename_ );
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message << std::endl;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const DOMException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message << std::endl;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (...) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Unknown or unexpected error.";
		throw std::runtime_error(msg.str());
	}
	
	return;
}

void XMLSceneBuilder::buildSpaces()
{
	try {
		// With a valid DOMDocument, its time to populate the Scene structure.
		XMLCh tagname[30];
		XMLString::transcode("space", tagname, 29);
		DOMNodeList* allSpaceItems = domDoc_->getElementsByTagName(tagname);
		
		// Loop over all of the spaces
		for (unsigned int spaceIndex = 0; spaceIndex < allSpaceItems->getLength(); ++spaceIndex) 
		{
			DOMNode* thisSpaceItem = allSpaceItems->item(spaceIndex);
		
			AttributesDirector attribDirector;

			SpaceAttributesBuilder spaceAttribBuilder(thisSpaceItem);
			attribDirector.SetAttributesBuilder( &spaceAttribBuilder );
			attribDirector.ConstructAttributes();

			AttributesPtr attrib = attribDirector.GetAttributes();

			// With successfully parsed attributes we can build an ODE space
			dSpaceID spaceID = NULL, parentID = NULL;

			// Get parent if one exists
			std::string spaceParent = attrib->getValAsStr("parent");
			if ( spaceParent.compare("_NOPARENT_") != 0 ) {
				parentID = scene_->getSpace( spaceParent );
			}
			
			// Get the type now so we build the correct type
			std::string spaceType = attrib->getValAsStr("type");				
			if ( spaceType.compare("simple") == 0 ) {
				spaceID = dSimpleSpaceCreate(parentID);
			} else if (spaceType.compare("hash") == 0) {
				dRealPtr center = attrib->getValAsVec("center", 3);
				dRealPtr extents = attrib->getValAsVec("extents", 3);
				float depth = attrib->getValAsReal("depth");
				
				spaceID = dQuadTreeSpaceCreate(parentID, center.get(), extents.get(), (int)depth);
			} else if (spaceType.compare("quadtree") == 0) {
				spaceID = dHashSpaceCreate(parentID);
				
				float minlevel = attrib->getValAsReal("minlevel");
				float maxlevel = attrib->getValAsReal("maxlevel");
				dHashSpaceSetLevels(spaceID, (int)minlevel, (int)maxlevel);
			}
		
			//Then create a new space object
			scene_->addSpace(attrib->getValAsStr("name"), spaceID);
		}
		
		// Now that all spaces have been created, look for <pair> tags to
		// create collision pairs
		XMLString::transcode("pair", tagname, 29);
		DOMNodeList* allPairItems = domDoc_->getElementsByTagName(tagname);
		
		for (unsigned int pairIndex = 0; pairIndex < allPairItems->getLength(); ++pairIndex)
		{
			DOMNode* thisPairItem = allPairItems->item(pairIndex);
			
			AttributesDirector attribDirector;
			
			PairAttributesBuilder pairAttribBuilder(thisPairItem);
			attribDirector.SetAttributesBuilder( &pairAttribBuilder );
			attribDirector.ConstructAttributes();

			AttributesPtr attrib = attribDirector.GetAttributes();

			// With successfully parsed attributes we can build an ODE space
			scene_->addCollisionPair(attrib->getValAsStr("space1"), attrib->getValAsStr("space2"));
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
	//std::cout << "Finished building spaces." << std::endl;
}

void XMLSceneBuilder::buildBodies()
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

void XMLSceneBuilder::buildGeoms()
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

//TransformList_t XMLSceneBuilder::parseTransform(DOMNode *node)
void XMLSceneBuilder::parseTransform(DOMNode *node, CompositeTransform* pRootTransform, Body *b)
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
				AttributesDirector attribDirector;
				TransformAttributesBuilder builder(thisChildItem);
				
				attribDirector.SetAttributesBuilder( &builder );
				attribDirector.ConstructAttributes();
				
				AttributesPtr attrib = attribDirector.GetAttributes();
				
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

CoordinateTransformPtr XMLSceneBuilder::parseSimpleTransform(DOMNode *node, Body *b) 
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

CoordinateTransformPtr XMLSceneBuilder::parseMarkerTransform(DOMNode *node, Body *b) 
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
