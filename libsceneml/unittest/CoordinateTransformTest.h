/*
 * CoordinateTransformTest.h
 *
 *  Created on: Aug 12, 2008
 *      Author: yamokosk
 */

#ifndef COORDINATETRANSFORMTEST_H_
#define COORDINATETRANSFORMTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <CoordinateTransform.h>

class CoordinateTransformTest: public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(CoordinateTransformTest);
	CPPUNIT_TEST(testSimpleTransforms);
	CPPUNIT_TEST(testMarkerTransforms);
	CPPUNIT_TEST_SUITE_END();
private:
	// Member variables here
public:
	void setUp();
	void tearDown();
protected:
	// Unittest declarations
	void testSimpleTransforms();
	void testMarkerTransforms();

};

#endif /* COORDINATETRANSFORMTEST_H_ */
