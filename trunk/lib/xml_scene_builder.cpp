#include <xode/xml_scene_builder.h>

#include <iostream>
#include <fstream>
//#include <matheval.h>

#include <xode/matrix.h>
#include <xode/MeshImport.h>

namespace xode {

XMLSceneBuilder::XMLSceneBuilder() :
	SceneBuilder(),
	filename_("")
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
	
}

XMLSceneBuilder::XMLSceneBuilder(const std::string& filename) :
	SceneBuilder(),
	filename_(filename)
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
	std::fstream fp(filename_.c_str(), std::fstream::in);  // declarations of streams fp_in and fp_out
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
	XMLString::transcode("LS", tempStr, 99);
	impl_ = DOMImplementationRegistry::getDOMImplementation(tempStr);
	parser_ = ((DOMImplementationLS*)impl_)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

	// optionally you can set some features on this builder
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
		domDoc_ = parser_->parseURI( filename_.c_str() );
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
		for (int spaceIndex = 0; spaceIndex < allSpaceItems->getLength(); ++spaceIndex) 
		{			DOMNode* thisSpaceItem = allSpaceItems->item(spaceIndex);
		
			AttributesDirector attribDirector;

			SpaceAttributesBuilder spaceAttribBuilder(thisSpaceItem);
			attribDirector.SetAttributesBuilder( &spaceAttribBuilder );
			attribDirector.ConstructAttributes();

			Attributes attrib = attribDirector.GetAttributes();

			// With successfully parsed attributes we can build an ODE space
			dSpaceID spaceID = NULL, parentID = NULL;

			// Get parent if one exists
			std::string spaceParent = attrib("parent");
			if ( spaceParent.compare("_NOPARENT_") != 0 ) {
				parentID = scene.get()->getSpace( spaceParent );
			}
			
			// Get the type now so we build the correct type
			std::string spaceType = attrib("type");				
			if ( spaceType.compare("simple") == 0 ) {
				spaceID = dSimpleSpaceCreate(parentID);
			} else if (spaceType.compare("hash") == 0) {
				float center[3], extents[3]; float depth;
				attrib.get("center", center);
				attrib.get("extents", extents);
				attrib.get("depth", &depth);
				
				spaceID = dQuadTreeSpaceCreate(parentID, center, extents, (int)depth);
			} else if (spaceType.compare("quadtree") == 0) {
				spaceID = dHashSpaceCreate(parentID);
				
				float minlevel, maxlevel;
				attrib.get("minlevel", &minlevel);
				attrib.get("maxlevel", &maxlevel);
				dHashSpaceSetLevels(spaceID, (int)minlevel, (int)maxlevel);
			}
		
			//Then create a new space object
			scene.get()->addSpace(attrib("name"), spaceID);
		}
		
		// Now that all spaces have been created, look for <pair> tags to
		// create collision pairs
		XMLString::transcode("pair", tagname, 29);
		DOMNodeList* allPairItems = domDoc_->getElementsByTagName(tagname);
		
		for (int pairIndex = 0; pairIndex < allPairItems->getLength(); ++pairIndex)
		{
			DOMNode* thisPairItem = allPairItems->item(pairIndex);
			
			AttributesDirector attribDirector;
			
			PairAttributesBuilder pairAttribBuilder(thisPairItem);
			attribDirector.SetAttributesBuilder( &pairAttribBuilder );
			attribDirector.ConstructAttributes();

			Attributes attrib = attribDirector.GetAttributes();

			// With successfully parsed attributes we can build an ODE space
			scene.get()->addCollisionPair(attrib("space1"), attrib("space2"));
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
		for (int bodyIndex = 0; bodyIndex < allBodyItems->getLength(); ++bodyIndex) 
		{			DOMNode* thisBodyItem = allBodyItems->item(bodyIndex);
		
			AttributesDirector attribDirector;

			BodyAttributesBuilder bodyAttribBuilder(thisBodyItem);
			attribDirector.SetAttributesBuilder( &bodyAttribBuilder );
			attribDirector.ConstructAttributes();

			Attributes attrib = attribDirector.GetAttributes();
			
			dBodyID b = dBodyCreate(scene.get()->getWorld());
			Body* body = scene.get()->createBody(attrib("name"), b);
			Body* prox = scene.get()->getBody(attrib("parent"));
			body->setProxObj(prox);
			prox->addDistBody(body);
			
			// Get/set transform data
			/*TransformList_t tlist = parseTransform(thisBodyItem);
			TransformList_t::iterator it = tlist.begin();
			for (; it != tlist.end(); ++it) {
				body->addTransform( (*it) );
				// If the transform is mutable, we need to notify the scene
				if ( (*it)->isMutable() ) scene.get()->addVariable((*it), body);
			}*/
			CompositeTransform* pRootTransform = new CompositeTransform();
			parseTransform(thisBodyItem, pRootTransform, body);
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
		for (int geomIndex = 0; geomIndex < allGeomItems->getLength(); ++geomIndex) 
		{			DOMNode* thisGeomItem = allGeomItems->item(geomIndex);
		
			AttributesDirector attribDirector;

			GeomAttributesBuilder geomAttribBuilder(thisGeomItem);
			attribDirector.SetAttributesBuilder( &geomAttribBuilder );
			attribDirector.ConstructAttributes();
			Attributes geomAttrib = attribDirector.GetAttributes();
			
			// Get geom's body
			DOMNode* thisGeomBodyItem = thisGeomItem->getParentNode();
			AttributesDirector bodyDirector;
			BodyAttributesBuilder bodyAttribBuilder(thisGeomBodyItem);
			bodyDirector.SetAttributesBuilder( &bodyAttribBuilder );
			bodyDirector.ConstructAttributes();
			Attributes bodyAttrib = bodyDirector.GetAttributes();
			Body* body = scene.get()->getBody(bodyAttrib("name"));
			
			// Get geom's space
			DOMNode* thisGeomSpaceItem = thisGeomBodyItem->getParentNode();
			AttributesDirector spaceDirector;
			SpaceAttributesBuilder spaceAttribBuilder(thisGeomSpaceItem);
			spaceDirector.SetAttributesBuilder( &spaceAttribBuilder );
			spaceDirector.ConstructAttributes();
			Attributes spaceAttrib = spaceDirector.GetAttributes();
			dSpaceID spaceID = scene.get()->getSpace(spaceAttrib("name"));
			
			// Construct the ODE object - based on type of course
			std::string type = geomAttrib("type");
			dGeomID g = NULL, t = NULL;
			POLYHEDRON *mesh = NULL;
			
			if (!type.compare("box")) {
				float length, width, height;
				geomAttrib.get("length", &length);
				geomAttrib.get("width", &width);
				geomAttrib.get("height", &height);
				g = dCreateBox(NULL, length, width, height);
			} else if (!type.compare("ccylinder")) {
				float length, radius;
				geomAttrib.get("length", &length);
				geomAttrib.get("radius", &radius);
				g = dCreateCCylinder(NULL, radius, length);
			} else if (!type.compare("cylinder")) {
				float length, radius;
				geomAttrib.get("length", &length);
				geomAttrib.get("radius", &radius);
				g = dCreateCylinder(NULL, radius, length);
			} else if (!type.compare("sphere")) {
				float radius;
				geomAttrib.get("radius", &radius);
				g = dCreateSphere(NULL, radius);
			} else if (!type.compare("plane")) {
				float nx, ny, nz, d;
				geomAttrib.get("normal_x", &nx);
				geomAttrib.get("normal_y", &ny);
				geomAttrib.get("normal_z", &nz);
				geomAttrib.get("d", &d);
				g = dCreatePlane(NULL, nx, ny, nz, d);
			} else if (!type.compare("mesh")) {				
				// Got to make a TriMeshObj!.. tough one.
				std::string filename = geomAttrib("filename");
				int pos = filename.find_last_of(".");
				std::string extension = filename.substr(pos+1);
				
				if (!extension.compare("obj")) {
					mesh = new POLYHEDRON();
					mesh->filename = filename;
					if ( importOBJ(mesh) != 0 )
					{
						std::ostringstream msg;
						msg << "importOBJ(): Returned an error!" << std::endl;
						throw std::runtime_error(msg.str());
					}
				} else if (!extension.compare("stl")) {
					mesh = new POLYHEDRON();
					mesh->filename = filename;
					if ( importSTL(mesh) != 0 )
					{
						std::ostringstream msg;
						msg << "importOBJ(): Returned an error!" << std::endl;
						throw std::runtime_error(msg.str());
					}
				} else {
					std::ostringstream msg;
					msg << extension << " is an unrecognized 3D model type. Currently only stl and obj files are supported." << std::endl;
					throw std::runtime_error(msg.str());
				}
				
				dTriMeshDataID Data = dGeomTriMeshDataCreate();
				dGeomTriMeshDataBuildSingle(Data,
                                 (void*)mesh->vertices, mesh->vertex_stride, mesh->vertex_count,
                                 (void*)mesh->indices,mesh->index_count,mesh->index_stride);
                    g = dCreateTriMesh (NULL, Data, NULL, NULL, NULL);
			}
			
			// At this point the geom has NOT been added to the correct space. First
			// we need to check if there is a transform between this geom and its body.
			// That effects who is added to what space.
			//TransformList_t tlist = parseTransform(thisGeomItem);
			CompositeTransform* pTransform = new CompositeTransform();
			parseTransform(thisGeomItem, pTransform);
			
			//if (tlist.size() > 0) {
			if (pTransform->size() > 0) {
				// There exists a transform between the body and geom.. therefore
				// we need a transform geom in ODE to accurately represent this.
				t = dCreateGeomTransform(NULL);
				dGeomTransformSetGeom(t, g);
				dGeomSetBody(t, body->id());
				dSpaceAdd(spaceID, t);
			} else {
				dGeomSetBody(g, body->id());
				dSpaceAdd(spaceID, g);
			}
			
			Geom* geom = scene.get()->createGeom(geomAttrib("name"), g, t);
			geom->setProxObj(body);
			geom->setMesh(mesh);
			geom->setCompositeTransform(pTransform);
			body->addGeom(geom);
			
			// Get color info
			dVector3 rgb;
			geomAttrib.get("color", rgb);
			geom->setColor(rgb);
			
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
		
	for (int c=0; c < allChildItems->getLength(); ++c)
	{
		DOMNode *thisChildItem = allChildItems->item(c);
		
		if (thisChildItem->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			char* pChildTagName = XMLString::transcode(thisChildItem->getNodeName());
						
			// Find the transform tag
			if (strcmp(pChildTagName, "transform") == 0) 
			{
				AttributesDirector attribDirector;
				TransformAttributesBuilder builder(thisChildItem);
				
				attribDirector.SetAttributesBuilder( &builder );
				attribDirector.ConstructAttributes();
				
				Attributes attrib = attribDirector.GetAttributes();
				
				std::string type = attrib("type");
				CoordinateTransform *pTransform = NULL;
				
				if ( !type.compare("simple") ) pTransform = parseSimpleTransform(thisChildItem, b);
				else if ( !type.compare("marker") ) pTransform = parseMarkerTransform(thisChildItem, b);
				
				if (pTransform != NULL) pRootTransform->add(pTransform);
			}
			
			delete [] pChildTagName;
		}
	}
	
	return;
}

CoordinateTransform* XMLSceneBuilder::parseSimpleTransform(DOMNode *node, Body *b) 
{
	CompositeTransform* pSimpleTransform = new CompositeTransform();
	
	DOMNodeList *allTransformItems = node->getChildNodes();

	for (int t=0; t < allTransformItems->getLength(); ++t) 
	{
		DOMNode *thisTransformItem = allTransformItems->item(t);		
		if (thisTransformItem->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			char* pTransformTagName = XMLString::transcode(thisTransformItem->getNodeName());

			// Parse the attributes.
			AttributesDirector attribDirector;
			Attributes attrib; AttributesBuilder *pBuilder = NULL;
			
			if ( strcmp(pTransformTagName, "translation") == 0 ) {
				pBuilder = new TranslationAttributesBuilder(thisTransformItem);
			} else if ( strcmp(pTransformTagName, "rotation") == 0 ) {
				pBuilder = new RotationAttributesBuilder(thisTransformItem);
			} else {
				std::ostringstream msg;
				msg << __FUNCTION__ << "(): Found an illegal tag " << pTransformTagName << " within a simple transformation.";
				throw std::runtime_error(msg.str());
			}
			attribDirector.SetAttributesBuilder( pBuilder );
			attribDirector.ConstructAttributes();
			attrib = attribDirector.GetAttributes();
			
			// Create the transform object
			SimpleTransform *aTransform = NULL;
			if ( strcmp(pTransformTagName, "translation") == 0 ) {
				aTransform = new SimpleTransform("translation", "_NODATA_");
			} else if ( strcmp(pTransformTagName, "rotation") == 0 ) {
				aTransform = new SimpleTransform("rotation", attrib("type"));
			}
			XMLString::release(&pTransformTagName);

			// Add value
			dReal *value = new dReal[3];
			attrib.get("value", value);
			aTransform->setData(value);
			
			// If value is mutable, register it with the scene			
			float isMutable = 0;
			attrib.get("mutable", &isMutable);
			if ( isMutable ) {
				if (b == NULL) {
					std::ostringstream msg;
					msg << __FUNCTION__ << "(): Found a mutable variable for a geometry. This is not allowed!";
					throw std::runtime_error(msg.str());
				} else {
					scene.get()->addMutableValue(attrib("name"), value, b);
				}
			}
			
			// Add simple transform to the composite
			pSimpleTransform->add( aTransform );
			
			delete pBuilder;
		}   
	}
				
	//return transList;
	return pSimpleTransform;
}

CoordinateTransform* XMLSceneBuilder::parseMarkerTransform(DOMNode *node, Body *b) 
{
	MarkerTransform* pMarkerTransform = new MarkerTransform();
	int nLocalCoord = 0, nGlobalCoord = 0;
	
	DOMNodeList *allTransformItems = node->getChildNodes();

	for (int t=0; t < allTransformItems->getLength(); ++t) 
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
			Attributes attrib = attribDirector.GetAttributes();
			
			dReal *value = new dReal[3];
			attrib.get("value", value);
			std::string type = attrib("type");
			if ( !type.compare("local") ){
				pMarkerTransform->addLocalCoord( value );
				nLocalCoord++;
			} else if ( !type.compare("global") ) {
				pMarkerTransform->addGlobalCoord( value );
				nGlobalCoord++;
			} else {
				delete [] value;
				std::ostringstream msg;
				msg << __FUNCTION__ << "(): Marker type " << type << " is illegal. It can either be local or global.";
				throw std::runtime_error(msg.str());
			}

			// If value is mutable, register it with the scene			
			float isMutable = 0;
			attrib.get("mutable", &isMutable);
			if ( isMutable ) {
				if (b == NULL) {
					std::ostringstream msg;
					msg << __FUNCTION__ << "(): Found a mutable variable for a geometry. This is not allowed!";
					throw std::runtime_error(msg.str());
				} else {
					scene.get()->addMutableValue(attrib("name"), value, b);
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
