/*
 * CoordinateTransformTest.cpp
 *
 *  Created on: Aug 12, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include "CoordinateTransformTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CoordinateTransformTest );

void CoordinateTransformTest::setUp()
{

}

void CoordinateTransformTest::tearDown()
{

}

void CoordinateTransformTest::testSimpleTransforms()
{
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
}

void CoordinateTransformTest::testMarkerTransforms()
{

}
