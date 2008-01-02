#ifndef _XODE_ATTRIBUTES_H_FILE_
#define _XODE_ATTRIBUTES_H_FILE_

#include <memory>
#include <map>
#include <vector>

#include <iostream>
#include <sstream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <muParser.h>
#define PARSER_CONST_PI  3.141592653589793238462643
//#include <matheval.h>

namespace xode {

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
