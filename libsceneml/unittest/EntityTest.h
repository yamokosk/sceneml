/*
 * EntityTest.h
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#include <cppunit/extensions/HelperMacros.h>

#ifndef CPP_UNIT_ENTITYTEST_H_
#define CPP_UNIT_ENTITYTEST_H_

class EntityTest: public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(EntityTest);
	CPPUNIT_TEST(testfunc);
	CPPUNIT_TEST_SUITE_END();
private:
	// Member variables here
public:
	void setUp();
	void tearDown();
protected:
	// Unittest declarations
	void testfunc();

};

#endif /* ENTITYTEST_H_ */
