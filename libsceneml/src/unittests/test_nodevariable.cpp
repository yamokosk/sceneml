/*
 * test_nodevariable.cpp
 *
 *  Created on: Jul 21, 2008
 *      Author: yamokosk
 */

#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <SceneMgr.h>
#include <Node.h>
#include <Variable.h>
#include <math/Vector.h>

using namespace sml;

BOOST_AUTO_TEST_CASE( basic_test )
{
	BOOST_TEST_MESSAGE( "Node/variable test." );

	SceneMgr mgr;

	Node* n1 = mgr.getRootNode()->createChild();
	Node* n2 = n1->createChild();
	Node* n3 = n2->createChild();
	Node* n4 = n2->createChild();
	Node* n5 = n4->createChild();

	Variable var1;
	var1.setType("translation");
	var1.subscribe(n2);
	var1.setVector( VectorFactory::Vector3( UNIT_X ) );
}
