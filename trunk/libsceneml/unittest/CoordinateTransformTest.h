/*
 * CoordinateTransformTest.h
 *
 *  Created on: Aug 12, 2008
 *      Author: yamokosk
 */

#ifndef COORDINATETRANSFORMTEST_H_
#define COORDINATETRANSFORMTEST_H_

// Logging
#include <log4cxx/logger.h>

#include <cppunit/extensions/HelperMacros.h>
#include <CoordinateTransform.h>

class CoordinateTransformTest: public CPPUNIT_NS::TestFixture
{
	static log4cxx::LoggerPtr logger;

	CPPUNIT_TEST_SUITE(CoordinateTransformTest);
	CPPUNIT_TEST(testSimpleTransforms);
	CPPUNIT_TEST(testCachedTransform);
	CPPUNIT_TEST(testComplexTransforms1);
	CPPUNIT_TEST(testComplexTransforms2);
	CPPUNIT_TEST(testTransformUpdate);
	CPPUNIT_TEST_SUITE_END();
private:
	// Member variables here
public:
	void setUp();
	void tearDown();
protected:
	// Unittest declarations
	void testSimpleTransforms();
	void testCachedTransform();
	void testComplexTransforms1();
	void testComplexTransforms2();
	void testTransformUpdate();

};

#endif /* COORDINATETRANSFORMTEST_H_ */
