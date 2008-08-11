/*
 * NodeTest.h
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#include <cppunit/extensions/HelperMacros.h>
#include <TinySG/Node.h>

#ifndef NODETEST_H_
#define NODETEST_H_

class NodeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( NodeTest );
	CPPUNIT_TEST( testCreateChild );
	CPPUNIT_TEST_SUITE_END();

private:
	Node *n1, *n2, *unnamed;

public:
	void setUp();
	void tearDown();

protected:
	// Test cases
	void testCreateChild();
};

#endif /* NODETEST_H_ */
