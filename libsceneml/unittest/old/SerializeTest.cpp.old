/*
 * SerializeTest.cpp
 *
 *  Created on: Sep 15, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include "SerializeTest.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cstdio>
// Boost stuff
//#include <Serializable.h>
//#include <Serializer.h>
#include <tinyxml/tinyxml.h>

using namespace log4cxx;

LoggerPtr SerializeTest::logger(Logger::getLogger("SerializeTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( SerializeTest );

#define ADD_ATTRIBUTE(attrib)	{														\
									stringstream ss;									\
									ss << attrib;										\
									pc->addPair( PropertyPair(#attrib, ss.str()) );		\
								}

struct Parameter
{
	Parameter() : name(""), value("") {}
	Parameter(const std::string& n, const std::string& v) : name(n), value(v) {}
	std::string name;
	std::string value;
};

class oArchive
{

public:
	oArchive() : level_(0), root_(NULL), currentElement_(NULL)
	{
		root_ = new TiXmlElement("TinySG");
		currentElement_ = root_;
	};
	virtual ~oArchive()
	{
		if (root_) delete root_;
	};

	template<class T>
	oArchive& operator&(T& obj)
	{
		TiXmlElement* newElement = new TiXmlElement( obj.getClassName() );
		std::cout << "Created " << obj.getClassName() << " tag." << std::endl;
		if ( currentElement_ != NULL) {
			//LOG4CXX_INFO(logger, "Linking " << obj.getClassName() << " to parent.");
			currentElement_->LinkEndChild( newElement );
			std::cout << "Linked " << obj.getClassName() << " to " << currentElement_->Value() << std::endl;
		}
		TiXmlElement *oldElement = currentElement_;
		currentElement_ = newElement;

		obj.serialize(*this);

		currentElement_ = oldElement;
		std::cout << "Closing out " << obj.getClassName() << std::endl;
		return *this;
	}

	unsigned int level_;
private:
	friend FILE* operator<<(FILE* fp, const oArchive& ar);

	TiXmlElement* root_;
	TiXmlElement* currentElement_;
};

template<>
oArchive& oArchive::operator&(Parameter& p)
{
	TiXmlElement* parameter = new TiXmlElement( "parameter" );
	if ( currentElement_ != NULL) {
		currentElement_->LinkEndChild( parameter );
		std::cout << "Linked parameter to " << currentElement_->Value() << std::endl;
	}
	parameter->SetAttribute("name", p.name);
	parameter->SetAttribute("value", p.value);
}

FILE* operator<<(FILE* fp, const oArchive& ar)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "" );
	doc.LinkEndChild( decl );
	doc.LinkEndChild( ar.root_ );
	doc.SaveFile( fp );
	return fp;
}


/////////////////////////////////////////////////////////////////////////////////////////////////

class iArchive
{

public:
	iArchive() : level_(0), doc_(NULL), root_(NULL), currentElement_(NULL)
	{ };
	virtual ~iArchive()
	{
		if (doc_) delete doc_;
	};

	template<class T>
	iArchive& operator&(T& obj)
	{
		// Assume we are currently at a parent node
		TiXmlElement *oldElement = currentElement_; // Hold onto the last good element
		currentElement_ = parent->FirstChildElement();
		if ( currentElement_ == NULL )
		{
			currentElement_ = currentElement->NextSiblingElement();
		}
		std::cout << "Current Tag: " << currentElement_->Value() << ", Obj Tag: " << obj.getClassName() << std::endl;
		obj.serialize(*this);

		currentElement_ = oldElement;
		return *this;
	}

	unsigned int level_;
private:
	friend FILE* operator>>(FILE* fp, iArchive& ar);

	TiXmlDocument* doc_;
	TiXmlElement* root_;
	TiXmlElement* currentElement_;
};

template<>
iArchive& iArchive::operator&(Parameter& p)
{
	p.name = currentElement_->Attribute("name");
	p.value = currentElement_->Attribute("value");

	// Parameters have no children by rule, so we need only increment pointer
	// to next valid element. If there are no more elements, we should back out to the parent
	// and go to its next sibling.
	currentElement_ = currentElement_->NextSiblingElement();
	if (currentElement_ == NULL)
	{
		currentElement_ = currentElement_->Parent()->NextSiblingElement();
	}
}

FILE* operator>>(FILE* fp, iArchive& ar)
{
	ar.doc_ = new TiXmlDocument();
	ar.doc_->LoadFile( fp );
	ar.currentElement_ = doc_->RootElement(); // Should be <TinySG>
	return fp;
}

class base
{
	friend class oArchive;
protected:
	virtual std::string getClassName() {return "base";}
	virtual void serialize(oArchive& ar)
	{
		//std::cout << "Entered base class serialize function" << std::endl;
		std::stringstream ss;
		ss << dval;
		Parameter p("dval", ss.str());
		ar & p;
	}

public:
	base() : dval(0.0) {}
	base(double b) : dval(b) {}
	virtual void a() = 0;
	double dval;
};

class derived : public base
{
	friend class oArchive;
protected:
	virtual std::string getClassName() {return "derived";}
	virtual void serialize(oArchive& ar)
	{
		//std::cout << "Entered derived class serialize function" << std::endl;
		base::serialize(ar);
		std::stringstream ss;
		ss << ival;
		Parameter p("ival", ss.str());
		ar & p;
	}
public:
	derived() : base(0.0), ival(0) {}
	derived(int a, double b) : base(b), ival(a) {}
	virtual void a() {};
	int ival;
};

class manager
{
	friend class oArchive;
protected:
	std::string getClassName() {return "manager";}
	void serialize(oArchive& ar)
	{
		Parameter p("color", "blue");
		ar & p;
		std::list<base*>::iterator iter = objs.begin();
		for (; iter != objs.end(); ++iter)
		{
			ar & *(*iter);
		}
	}
public:
	std::list<base*> objs;
};

void SerializeTest::setUp()
{

}

void SerializeTest::tearDown()
{

}

void SerializeTest::testSimpleClass()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	// Creat some data to save
	derived d1(1, 1.0), d2(2, 3.0);

	// Write some data
	{
		oArchive ar;
		ar & d1 & d2;
		FILE* fp = fopen("simpleclass.xml", "w");
		fp << ar;
		fclose(fp);
	}

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}

void SerializeTest::testManagerClass()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	// Creat some data to save
	derived d1(1, 1.0), d2(2, 3.0);

	manager mgr;
	mgr.objs.push_back( &d1 );
	mgr.objs.push_back( &d2 );

	// Write some data
	{
		oArchive ar;
		ar & mgr;
		FILE* fp = fopen("manager.xml", "w");
		fp << ar;
		fclose(fp);
	}

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}


