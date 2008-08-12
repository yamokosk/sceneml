/*
 * NodeTest.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include "NodeTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( NodeTest );

void NodeTest::setUp()
{
	n1 = new Node("n1");
	n2 = new Node("n2");
	unnamed = new Node();
}

void NodeTest::tearDown()
{
	delete n1;
	delete n2;
	delete unnamed;
}

// Test cases
void NodeTest::testAddChild()
{
	n1->addChild(n2);
}
