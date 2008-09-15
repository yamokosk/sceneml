/*
 * SerializeTest.cpp
 *
 *  Created on: Sep 15, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include "SerializeTest.h"
#include <sstream>

using namespace log4cxx;

LoggerPtr SerializeTest::logger(Logger::getLogger("SerializeTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( SerializeTest );


struct Node : public TinySG::Serializable
{
	Node(const std::string& name, double val=0.0, Node* parent=NULL) :
		name_(name), val_(val), parent_(parent) {}
	virtual void getAttributes(TinySG::PropertyCollection& pc)
	{
		pc.addPair( TinySG::PropertyPair("name", name_.c_str()) );
		stringstream str; str << val_;
		pc.addPair( TinySG::PropertyPair("value", str.str().c_str()) );
		if ( parent_ != NULL )
			pc.addPair( TinySG::PropertyPair("parent", parent_->name_.c_str()) );
	}

	virtual ComplexProperties getProperties()
	{
		ComplexProperties props;
		return props;
	}

	virtual std::string getClassName() {return "node";}

	std::string name_;
	double val_;
	Node* parent_;
};

void SerializeTest::setUp()
{

}

void SerializeTest::tearDown()
{

}

void SerializeTest::testSerialize()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	// Creat some data to save
	Node n1("n1",3.2);
	Node n2("n2",-1.2,&n1);
	Node n3("n3",1103.2,&n2);
	Node n4("n4",103.0,&n1);

	// Now lets save some data
	TinySG::Serializer serializer;

	serializer.serialize(&n1);
	serializer.serialize(&n2);
	serializer.serialize(&n3);
	serializer.serialize(&n4);

	serializer.save("out.xml");

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}


