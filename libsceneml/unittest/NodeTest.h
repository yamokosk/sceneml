/*
 * NodeTest.h
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#include <Node.h>
#include <cppunit/extensions/HelperMacros.h>

#ifndef NODETEST_H_
#define NODETEST_H_

class NodeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( NodeTest );
	CPPUNIT_TEST( testEquality );
	CPPUNIT_TEST( testAddition );
	CPPUNIT_TEST_SUITE_END();

private:
	Node *n1, *n2, *n3;

public:
	void setUp();
	void tearDown();

protected:
	// Test cases
	void testEquality();
	void testAddition();
};

#endif /* NODETEST_H_ */
