// Boost.Test
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "smlSignals.h"
#include <iostream>

class HelloObs : public sceneml::Observer
{
public:
	HelloObs(sceneml::Subject& s) : sceneml::Observer(s) {};

	virtual void notify(bool bExtended) const
	{
		std::cout << "Hello ";
	}
};

class WorldObs : public sceneml::Observer
{
public:
	WorldObs(sceneml::Subject& s) : sceneml::Observer(s) {};

	virtual void notify(bool bExtended) const
	{
		std::cout << " world!";
	}
};

BOOST_AUTO_TEST_CASE( test1 )
{
	sceneml::Subject obj;
}

BOOST_AUTO_TEST_CASE( sub_with_observer )
{
	sceneml::Subject sub;
	
	HelloObs o1(sub);
	WorldObs o2(sub);
	
	sub.notify();
}