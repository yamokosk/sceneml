/*
 * test_mathfactories.cpp
 *
 *  Created on: Jul 22, 2008
 *      Author: yamokosk
 */

#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <math/Vector.h>
#include <math/Matrix.h>
#include <math/Quaternion.h>

using namespace sml;

BOOST_AUTO_TEST_CASE( vector3_factories_test )
{
	BOOST_TEST_MESSAGE( "Vector3 factories test" );

	ColumnVector v1(3), v2(3);

	v1 = math::VectorFactory::Vector3( math::UNIT_X );
	v2 << 1. << 0. << 0.;
	BOOST_CHECK( v1 == v2);

	v1 = math::VectorFactory::Vector3( math::UNIT_Y );
	v2 << 0. << 1. << 0.;
	BOOST_CHECK( v1 == v2);

	v1 = math::VectorFactory::Vector3( math::UNIT_Z );
	v2 << 0. << 0. << 1.;
	BOOST_CHECK( v1 == v2);

	v1 = math::VectorFactory::Vector3( math::ZERO );
	v2 << 0. << 0. << 0.;
	BOOST_CHECK( v1 == v2);

	v1 = math::VectorFactory::Vector3( math::ONES );
	v2 << 1. << 1. << 1.;
	BOOST_CHECK( v1 == v2);
}

BOOST_AUTO_TEST_CASE( vector4_factories_test )
{
	BOOST_TEST_MESSAGE( "Vector4 factories test" );

	ColumnVector v1(4), v2(4);

	v1 = math::VectorFactory::Vector4( math::UNIT_X );
	v2 << 1. << 0. << 0. << 1.;
	BOOST_CHECK( v1 == v2);

	v1 = math::VectorFactory::Vector4( math::UNIT_Y );
	v2 << 0. << 1. << 0. << 1.;
	BOOST_CHECK( v1 == v2);

	v1 = math::VectorFactory::Vector4( math::UNIT_Z );
	v2 << 0. << 0. << 1. << 1.;
	BOOST_CHECK( v1 == v2);

	v1 = math::VectorFactory::Vector4( math::ZERO );
	v2 << 0. << 0. << 0. << 1.;
	BOOST_CHECK( v1 == v2);

	v1 = math::VectorFactory::Vector4( math::ONES );
	v2 << 1. << 1. << 1. << 1.;
	BOOST_CHECK( v1 == v2);
}

BOOST_AUTO_TEST_CASE( matrix3x3_factories_test )
{
	BOOST_TEST_MESSAGE( "Matrix3x3 factories test" );

	Matrix m1(3,3), m2(3,3);

	m1 = math::MatrixFactory::Matrix3x3( math::IDENTITY );
	m2 = IdentityMatrix(3);
	BOOST_CHECK( m1 == m2);

	m1 = math::MatrixFactory::Matrix3x3( math::ZERO );
	m2 = 0.;
	BOOST_CHECK( m1 == m2);
}

BOOST_AUTO_TEST_CASE( matrix4x4_factories_test )
{
	BOOST_TEST_MESSAGE( "Matrix4x4 factories test" );

	Matrix m1(4,4), m2(4,4);

	m1 = math::MatrixFactory::Matrix4x4( math::IDENTITY );
	m2 = IdentityMatrix(4);
	BOOST_CHECK( m1 == m2);

	m1 = math::MatrixFactory::Matrix4x4( math::ZERO );
	m2 = 0.;
	BOOST_CHECK( m1 == m2);
}

BOOST_AUTO_TEST_CASE( quaternion_factories_test )
{
	BOOST_TEST_MESSAGE( "Quaternion factories test" );

	math::Quaternion q1, q2;

	q1 = math::QuaternionFactory::Quat( math::IDENTITY );
	q2 = math::Quaternion(1.,0.,0.,0.);
	BOOST_CHECK( q1 == q2);

	q1 = math::QuaternionFactory::Quat( math::ZERO );
	q2 = math::Quaternion(0.,0.,0.,0.);
	BOOST_CHECK( q1 == q2);
}
