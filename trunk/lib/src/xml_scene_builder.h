#ifndef _XODE_XML_SCENE_BUILDER_H_FILE_
#define _XODE_XML_SCENE_BUILDER_H_FILE_

#include <xode/scene_builder.h>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <xode/xml_attributes.h>

#include <string>
#include <map>

#include <sstream>
#include <stdexcept>


namespace xode {

XERCES_CPP_NAMESPACE_USE

// ConcreteBuilder
class XMLSceneBuilder : public SceneBuilder
{
public:
	XMLSceneBuilder(const std::string& filename);
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
	CoordinateTransform* parseSimpleTransform(DOMNode *node, Body *b=NULL);
	CoordinateTransform* parseMarkerTransform(DOMNode *node, Body *b=NULL);
	void isFileValid();
	
	DOMImplementation* impl_;
	DOMBuilder* parser_;
	DOMDocument* domDoc_;
	
	std::string filename_;
};

};

#endif
