/*
 * test_observer.cpp
 *
 *  Created on: Jul 21, 2008
 *      Author: yamokosk
 */

#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <boost/cast.hpp>

#include <iostream>

//#include <Observer.h>
#include <Subject.h>

using namespace sml;

/*BOOST_AUTO_TEST_CASE( simple_observer_test )
{
	Subject subject;

	subject.subscribe(NULL);
}*/
class HelloObserver : public sml::Observer
{
public:
	HelloObserver() : Observer() {}
	virtual ~HelloObserver() {};

	virtual void update(Subject* sub)
	{
		std::cout << "Hello ";
	}
};

class WorldObserver : public sml::Observer
{
public:
	WorldObserver() : Observer() {}
	virtual ~WorldObserver() {};

	virtual void update(Subject* sub)
	{
		std::cout << "World!" << std::endl;
	}
};

class Numbers : public sml::Subject
{
public:
	Numbers() : Subject(), a_(0), b_(0.0) {}
	virtual ~Numbers() {};

	void setInt(int a)
	{
		a_ = a;
		this->notify();
	}
	void setFloat(float b)
	{
		b_ = b;
		this->notify();
	}

	int a_;
	float b_;
};

class FloatObserver : public sml::Observer
{
public:
	FloatObserver() : Observer() {};
	virtual ~FloatObserver() {};

	virtual void update(Subject* sub)
	{
		Numbers* num = boost::polymorphic_downcast<Numbers*>(sub);
		std::cout << "float = " << num->b_ << std::endl;
	}
};

class IntObserver : public sml::Observer
{
public:
	IntObserver() : Observer() {};
	virtual ~IntObserver() {};

	virtual void update(Subject* sub)
	{
		Numbers* num = boost::polymorphic_downcast<Numbers*>(sub);
		std::cout << "int = " << num->a_ << std::endl;
	}
};

BOOST_AUTO_TEST_CASE( simple_observer_test )
{
	BOOST_TEST_MESSAGE( "Simple observer test." );

	HelloObserver obs1;
	WorldObserver obs2;

	Subject sub;
	sub.subscribe(&obs1);
	sub.subscribe(&obs2);
	sub.notify();

	sub.unsubscribe(&obs1);
	sub.notify();
}

BOOST_AUTO_TEST_CASE( complex_observer_test )
{
	BOOST_TEST_MESSAGE( "Complex observer test." );

	FloatObserver obs1;
	IntObserver obs2;

	Numbers sub;
	sub.subscribe(&obs1);
	sub.subscribe(&obs2);
	sub.notify();

	sub.setInt(1);
	sub.setFloat(2.0);
	sub.setFloat(3.0);
	sub.setInt(4);

	sub.unsubscribe(&obs1);
	sub.unsubscribe(&obs2);
	sub.setFloat(100.0);
	sub.setInt(100);
}