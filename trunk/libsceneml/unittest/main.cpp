/*
 * main.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main( int argc, char **argv)
{
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest( registry.makeTest() );
	bool wasSucessful = runner.run( "", false );
	return !wasSucessful;
}
