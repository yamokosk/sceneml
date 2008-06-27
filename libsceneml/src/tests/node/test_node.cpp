#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <smlNode.h>
#include <smlException.h>

using namespace sml;

BOOST_AUTO_TEST_CASE( constructor_tests )
{
	BOOST_TEST_MESSAGE( "Testing node constructors." );
    
	Node n1(NULL);
    BOOST_CHECK_EQUAL( n1.getName().compare("Unnamed_1"), 0 );
	BOOST_CHECK( n1.getManager() == 0 );

	SceneMgr mgr;
	Node n2(&mgr, "test");
	BOOST_CHECK_EQUAL( n2.getName().compare("test"), 0 );
	BOOST_CHECK( n2.getManager() == &mgr );
}

BOOST_AUTO_TEST_CASE( creator_tests )
{
	BOOST_TEST_MESSAGE( "Testing node create methods." );
    
	// Functions to test:
	//	createChild()
	//	addChild()
	//	numChildren()
	//	getChild()
 	
	SceneMgr mgr;
	Node n1(&mgr, "node1");
	Node n2(&mgr, "node2");
	
	//	addChild(), numChildren(), getChild(), getParent()
	n1.addChild(&n2);
	BOOST_CHECK_EQUAL( n1.numChildren(), 1 );
	BOOST_CHECK_EQUAL( n1.getChild(0)->getName().compare("node2"), 0 );
	BOOST_CHECK_EQUAL( n1.getChild("node2")->getName().compare("node2"), 0 );
	BOOST_CHECK_EQUAL( n2.getParent()->getName().compare("node1"), 0 );
	BOOST_CHECK_THROW( n1.addChild(&n2), sml::Exception );
	
	// removeChild(), numChildren()
	n1.removeChild(&n2);
	BOOST_CHECK_EQUAL( n1.numChildren(), 0 );
	BOOST_CHECK_THROW( n1.removeChild("node2"), sml::Exception )
}

BOOST_AUTO_TEST_CASE( pose_tests )
{
	BOOST_TEST_MESSAGE( "Testing node create methods." );
    
	// Functions to test:
	//	createChild()
	//	addChild()
	//	numChildren()
	//	getChild()
 	
	SceneMgr mgr;
	Node n1(&mgr, "node1");
		
	//	addChild(), numChildren(), getChild(), getParent()
	n1.addChild(&n2);
	BOOST_CHECK_EQUAL( n1.numChildren(), 1 );
	BOOST_CHECK_EQUAL( n1.getChild(0)->getName().compare("node2"), 0 );
	BOOST_CHECK_EQUAL( n1.getChild("node2")->getName().compare("node2"), 0 );
	BOOST_CHECK_EQUAL( n2.getParent()->getName().compare("node1"), 0 );
	BOOST_CHECK_THROW( n1.addChild(&n2), sml::Exception );
	
	// removeChild(), numChildren()
	n1.removeChild(&n2);
	BOOST_CHECK_EQUAL( n1.numChildren(), 0 );
	BOOST_CHECK_THROW( n1.removeChild("node2"), sml::Exception )
}