/*
 * NodeTest.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include <math/Vector.h>
#include "NodeTest.h"

using namespace log4cxx;

LoggerPtr NodeTest::logger(Logger::getLogger("NodeTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( NodeTest );


string convBase(unsigned long v, long base)
{
	string digits = "0123456789abcdef";
	string result;
	if((base < 2) || (base > 16)) {
		result = "Error: base out of range.";
	}
	else {
		do {
			result = digits[v % base] + result;
			v /= base;
		}
		while(v);
	}
	return result;
}



void NodeTest::setUp()
{
	n1 = dynamic_cast<TinySG::Node*>( fact.createInstance("n1", NULL, NULL) );
	n2 = dynamic_cast<TinySG::Node*>( fact.createInstance("n2", NULL, NULL) );
	n3 = dynamic_cast<TinySG::Node*>( fact.createInstance("n3", NULL, NULL) );
	n4 = dynamic_cast<TinySG::Node*>( fact.createInstance("n4", NULL, NULL) );
	n2_copy = dynamic_cast<TinySG::Node*>( fact.createInstance("n2", NULL, NULL) );

	/* Create tree
			n1
		  /	   \
		n2		n3
				|
				n4
	 */
	n1->addChild(n2);
	n1->addChild(n3);
	n3->addChild(n4);
	n1->update();
}

void NodeTest::tearDown()
{
	fact.destroyInstance(n1);
	fact.destroyInstance(n2);
	fact.destroyInstance(n3);
	fact.destroyInstance(n4);
	fact.destroyInstance(n2_copy);
}

void NodeTest::testNumChildren()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	CPPUNIT_ASSERT( n1->numChildren() == 2 );
	CPPUNIT_ASSERT( n2->numChildren() == 0 );
	CPPUNIT_ASSERT( n3->numChildren() == 1 );
	CPPUNIT_ASSERT( n4->numChildren() == 0 );
}

void NodeTest::testGetChildByIndex()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	CPPUNIT_ASSERT( n1->getChild(0) == n2 );
	CPPUNIT_ASSERT( n1->getChild(1) == n3 );
	CPPUNIT_ASSERT( n3->getChild(0) == n4 );
}

void NodeTest::testGetChildByName()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	CPPUNIT_ASSERT( n1->getChild("n2") == n2 );
	CPPUNIT_ASSERT( n1->getChild("n3") == n3 );
	CPPUNIT_ASSERT( n3->getChild("n4") == n4 );
}

void NodeTest::testRemoveChildByIndex()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	CPPUNIT_ASSERT( n1->removeChild(1) == n3 );
}

void NodeTest::testRemoveChildByName()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	CPPUNIT_ASSERT( n1->removeChild("n3") == n3 );
}

void NodeTest::testRemoveAllChildren()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	n3->removeAllChildren();
	CPPUNIT_ASSERT( n3->numChildren() == 0 );
}

void NodeTest::testTranslateRelativeToLocal()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	n3->translate( TinySG::VectorFactory::Vector3( TinySG::UNIT_X ), TinySG::Node::TS_LOCAL );
	n1->update();
}

void NodeTest::testTranslateRelativeToParent()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	n3->translate( TinySG::VectorFactory::Vector3( TinySG::UNIT_X ), TinySG::Node::TS_PARENT );
	n1->update();
}

void NodeTest::testTranslateRelativeToWorld()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	n3->translate( TinySG::VectorFactory::Vector3( TinySG::UNIT_X ), TinySG::Node::TS_WORLD );
	n1->update();
}

void NodeTest::testRotateRelativeToLocal()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	n3->rotate(TinySG::VectorFactory::Vector3( TinySG::UNIT_X ), TinySG::pi/3.0, TinySG::Node::TS_LOCAL);
	n1->update();
}

void NodeTest::testRotateRelativeToParent()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	n3->rotate(TinySG::VectorFactory::Vector3( TinySG::UNIT_X ), TinySG::pi/3.0, TinySG::Node::TS_PARENT);
	n1->update();
}

void NodeTest::testRotateRelativeToWorld()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	n3->rotate(TinySG::VectorFactory::Vector3( TinySG::UNIT_X ), TinySG::pi/3.0, TinySG::Node::TS_WORLD);
	n1->update();
}

void NodeTest::testProcessUpdates()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	std::cout << *n2;
	n2->setPosition(4.0, 0.0, -3.0);
	n1->update();
	std::cout << *n2;
}

// Exception test cases
void NodeTest::testAddChildWithParent()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);
	n1->addChild(n4);
}

void NodeTest::testAddChildWithSameName()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	n1->addChild(n2_copy);
}

void NodeTest::testGetChildByBadIndex()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);
	n1->getChild(3);
}

void NodeTest::testGetChildByBadName()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);
	n1->getChild("unknown");
}

void NodeTest::testRemoveChildByBadIndex()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);
	n1->removeChild(3);
}

void NodeTest::testRemoveChildByBadName()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);
	n1->removeChild("unknown");
}
