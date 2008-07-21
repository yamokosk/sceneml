/*
 * test_mathexpression.cpp
 *
 *  Created on: Jul 21, 2008
 *      Author: yamokosk
 */

#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <MathExpression.h>

using namespace sml;

BOOST_AUTO_TEST_CASE( basic_test )
{
	BOOST_TEST_MESSAGE( "Basic expression test." );

	std::cout << "Testing: 2.0 * 2.0" << std::endl;
	math::MathExpression expr("2.0 * 2.0");
	math::Real val = expr.getAsReal();

	BOOST_CHECK_EQUAL( val, 4.0 );

	std::cout << "Testing: 2.0 2.0*3 3-1" << std::endl;
	math::MathExpression vecexpr("2.0 2.0*3 3-1");

	ColumnVector vec = vecexpr.getAsVector(3);

	BOOST_CHECK_EQUAL( vec(1), 2.0 );
	BOOST_CHECK_EQUAL( vec(2), 6.0 );
	BOOST_CHECK_EQUAL( vec(3), 2.0 );
}
