/*
 * ObjectManagerTest.cpp
 *
 *  Created on: Sep 15, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include "ObjectManagerTest.h"

using namespace log4cxx;

LoggerPtr ObjectManagerTest::logger(Logger::getLogger("ObjectManagerTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( ObjectManagerTest );


class MyObject : public TinySG::Object
{
public:
	MyObject(double d) : TinySG::Object(), val(d) {};
	virtual ~MyObject() {};

	double val;
};

class MyObjectFactory : public TinySG::ObjectFactory
{
protected:
	virtual TinySG::Object* createInstanceImpl(const TinySG::PropertyCollection* params)
	{
		return (new MyObject(1.0));
	}

public:
	MyObjectFactory() : TinySG::ObjectFactory("MyObject") {};
	virtual ~MyObjectFactory() {};
	virtual void destroyInstance(TinySG::Object* obj) {delete obj;}
};

class MyManager : public TinySG::ObjectManager
{
public:
	MyManager()
	{
		this->registerFactory( new MyObjectFactory() );
	}
	virtual ~MyManager() {};

	MyObject* createMyObject(const std::string& name)
	{
		return (MyObject*)createObject(name, "MyObject");
	}
	MyObject* getMyObject(const std::string name)
	{
		return (MyObject*)getObject(name, "MyObject");
	}
	void getMyCollection(const std::string type)
	{
		getCollection(type);
	}
	void addFactory( TinySG::ObjectFactory* fact)
	{
		registerFactory( fact );
	}
	void deleteMyCollection( const std::string& type )
	{
		destroyCollection(type);
	}
	void deleteMyObject( const std::string& name )
	{
		destroyObject( name, "MyObject" );
	}
};

void ObjectManagerTest::setUp()
{

}

void ObjectManagerTest::tearDown()
{

}

void ObjectManagerTest::testSimpleManager()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	MyManager mgr;

	MyObject* one = mgr.createMyObject("one");
	MyObject* two = mgr.createMyObject("two");
	MyObject* three = mgr.createMyObject("three");

	CPPUNIT_ASSERT( mgr.getMyObject("one") == one );
	CPPUNIT_ASSERT( mgr.getMyObject("two") == two );
	CPPUNIT_ASSERT( mgr.getMyObject("three") == three );

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}

void ObjectManagerTest::testCreateDuplicateObject()
{
	MyManager mgr;
	MyObject* one = mgr.createMyObject("one");
	MyObject* two = mgr.createMyObject("one"); // Exception
}

void ObjectManagerTest::testDestroyInvalidObject()
{
	MyManager mgr;
	mgr.deleteMyObject("one"); // Exception
}

void ObjectManagerTest::testGetInvalidObject()
{
	MyManager mgr;
	mgr.getMyObject("one"); // Exception
}

void ObjectManagerTest::testRegisterDuplicateFactory()
{
	MyManager mgr;
	mgr.addFactory( new MyObjectFactory() ); // Exception
}

void ObjectManagerTest::testDestroyInvalidCollection()
{
	MyManager mgr;
	mgr.deleteMyCollection("Double"); // Exception
}

void ObjectManagerTest::testGetInvalidCollection()
{
	MyManager mgr;
	mgr.getMyCollection("Double"); // Exception
}


