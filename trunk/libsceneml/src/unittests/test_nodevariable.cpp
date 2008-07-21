/*
 * test_nodevariable.cpp
 *
 *  Created on: Jul 21, 2008
 *      Author: yamokosk
 */

#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <Node.h>

using namespace sml;

BOOST_AUTO_TEST_CASE( basic_test )
{
	BOOST_TEST_MESSAGE( "Node/variable test." );

	// First create a variable
	Variable var1;
	var1.addPair( RequiredProperty("type","translation") );
	var1.addPair( RequiredProperty("value","1.0 2.0 3.0") );

	Node node(NULL);
	node.addSubject(var1);
}
