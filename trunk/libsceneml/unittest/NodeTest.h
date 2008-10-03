/*
 * NodeTest.h
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#ifndef NODETEST_H_
#define NODETEST_H_

#include <cppunit/extensions/HelperMacros.h>

class NodeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( NodeTest );
	CPPUNIT_TEST( testAddChild );
	CPPUNIT_TEST_SUITE_END();

private:
//	Node *n1, *n2, *unnamed;

public:
	void setUp();
	void tearDown();

protected:
	// Test cases
	void testAddChild();
};

#endif /* NODETEST_H_ */
