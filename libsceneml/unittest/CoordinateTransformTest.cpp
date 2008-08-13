/*
 * CoordinateTransformTest.cpp
 *
 *  Created on: Aug 12, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include "CoordinateTransformTest.h"
#include <sstream>

using namespace log4cxx;

LoggerPtr CoordinateTransformTest::logger(Logger::getLogger("CoordinateTransformTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( CoordinateTransformTest );

void CoordinateTransformTest::setUp()
{

}

void CoordinateTransformTest::tearDown()
{

}

void CoordinateTransformTest::testSimpleTransforms()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	TinySG::Real ang = TinySG::pi/3.0;
	TinySG::SimpleRotation r(TinySG::UNIT_X, ang);
	Matrix m = r.getTransform();
	CPPUNIT_ASSERT( m(1,1) == 1.0 );
	CPPUNIT_ASSERT( m(1,2) == 0.0 );
	CPPUNIT_ASSERT( fabs(m(2,2) - cos(ang)) < 1e-6 );
	CPPUNIT_ASSERT( fabs(m(2,3) + sin(ang)) < 1e-6 );

	TinySG::SimpleTranslation t(0.0, 1.0, 2.0);
	m = t.getTransform();
	CPPUNIT_ASSERT( m(1,1) == 1.0 );
	CPPUNIT_ASSERT( m(1,2) == 0.0 );
	CPPUNIT_ASSERT( m(1,4) == 0.0 );
	CPPUNIT_ASSERT( m(3,4) == 2.0 );

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}

void CoordinateTransformTest::testCachedTransform()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	// Checking first a simple rotation
	TinySG::SimpleRotation r(TinySG::UNIT_X, TinySG::zero);
	LOG4CXX_INFO(logger, "Calling r.getTransform first time.");
	r.getTransform();
	LOG4CXX_INFO(logger, "Calling r.getTransform second time.");
	r.getTransform();

	// Now a more complex composite transform
	TinySG::CompositeTransform c1;
	c1.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_Z,TinySG::halfPi) ) );
	c1.add( NEW_TRANSFORM( TinySG::SimpleTranslation(0.0, 1.0, 2.0) ) );
	c1.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_X, TinySG::pi) ) );

	LOG4CXX_INFO(logger, "Calling c1.getTransform first time.");
	c1.getTransform();
	LOG4CXX_INFO(logger, "Calling c1.getTransform second time.");
	c1.getTransform();

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}

void CoordinateTransformTest::testComplexTransforms1()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	TinySG::Real tx = TinySG::pi/3.0;
	TinySG::Real ty = TinySG::pi/6.0;
	TinySG::Real tz = -TinySG::pi/4.0;

	// Create Euler sequence rotation
	LOG4CXX_INFO(logger, "Creating EulerRotation transform object r");
	TinySG::EulerRotation r(TinySG::ZXY, tx, ty, tz);

	// Do the same rotation but using composite transform of simple rotations
	LOG4CXX_INFO(logger, "Creating Composite transform object c1");
	TinySG::CompositeTransform c1;
	c1.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_Z, tz) ) );
	c1.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_X, tx) ) );
	c1.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_Y, ty) ) );

	LOG4CXX_INFO(logger, "Creating Composite transform object c2");
	TinySG::CompositeTransform c2;
	c2.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_X, tx) ) );
	c2.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_Y, ty) ) );
	c2.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_Z, tz) ) );

	// Should be equal
	LOG4CXX_INFO(logger, "Comparing c1 and r");
	CPPUNIT_ASSERT( c1.getTransform() == r.getTransform() );

	// Should not be equal
	LOG4CXX_INFO(logger, "Comparing c2 and r");
	CPPUNIT_ASSERT( c2.getTransform() != r.getTransform() );

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}

void CoordinateTransformTest::testComplexTransforms2()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	TinySG::Real tx = TinySG::pi/3.0;
	TinySG::Real ty = TinySG::pi/6.0;
	TinySG::Real tz = -TinySG::pi/4.0;

	// Create Euler sequence rotation
	LOG4CXX_INFO(logger, "Creating EulerRotation transform object r");
	TinySG::EulerRotation r(TinySG::XYZ, tx, ty, tz);

	// Do the same rotation but using composite transform of simple rotations
	LOG4CXX_INFO(logger, "Creating Composite transform object c1");
	TinySG::CompositeTransform c1;
	c1.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_X, tx) ) );
	c1.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_Y, ty) ) );
	c1.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_Z, tz) ) );

	LOG4CXX_INFO(logger, "Creating Composite transform object c2");
	TinySG::CompositeTransform c2;
	c2.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_Z, tz) ) );
	c2.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_X, tx) ) );
	c2.add( NEW_TRANSFORM( TinySG::SimpleRotation(TinySG::UNIT_Z, tz) ) );

	// Should be equal
	LOG4CXX_INFO(logger, "Comparing c1 and r");
	CPPUNIT_ASSERT( c1.getTransform() == r.getTransform() );

	// Should not be equal
	LOG4CXX_INFO(logger, "Comparing c2 and r");
	CPPUNIT_ASSERT( c2.getTransform() != r.getTransform() );

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}

void CoordinateTransformTest::testTransformUpdate()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	TinySG::Real a1 = TinySG::pi/3.0;
	TinySG::Real a2 = TinySG::pi/6.0;
	TinySG::Real a3 = -TinySG::pi/4.0;
	TinySG::Real a4 = -TinySG::pi/8.0;

	TinySG::SimpleRotation* r1 = new TinySG::SimpleRotation(TinySG::UNIT_X, a1);
	TinySG::SimpleRotation* r2 = new TinySG::SimpleRotation(TinySG::UNIT_Y, a2);
	TinySG::SimpleRotation* r3 = new TinySG::SimpleRotation(TinySG::UNIT_X, a3);
	TinySG::SimpleRotation* r4 = new TinySG::SimpleRotation(TinySG::UNIT_Y, a4);

	LOG4CXX_INFO(logger, "Creating Composite transform object c1");
	TinySG::CompositeTransform c1;
	c1.add( TinySG::CoordinateTransformPtr(r1) );
	c1.add( TinySG::CoordinateTransformPtr(r2) );

	LOG4CXX_INFO(logger, "Creating Composite transform object c2");
	TinySG::CompositeTransform c2;
	c2.add( TinySG::CoordinateTransformPtr(r3) );
	c2.add( TinySG::CoordinateTransformPtr(r4) );

	// Should not be equal
	LOG4CXX_INFO(logger, "Comparing c1 and c2");
	CPPUNIT_ASSERT( c2.getTransform() != c1.getTransform() );

	// Change c1 angles to match c2
	r1->setAngle(a3);
	r2->setAngle(a4);

	// Should be equal
	LOG4CXX_INFO(logger, "Comparing c1 and c2");
	CPPUNIT_ASSERT( c2.getTransform() == c1.getTransform() );

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}
