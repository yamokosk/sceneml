/*
 * SerializeTest.cpp
 *
 *  Created on: Sep 15, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include "SerializeTest.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>

// Boost stuff
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/version.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

using namespace log4cxx;

LoggerPtr SerializeTest::logger(Logger::getLogger("SerializeTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( SerializeTest );

class base
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int file_version)
	{
		ar & BOOST_SERIALIZATION_NVP(adouble);
	}
public:
	derived(double b) : adouble(b) {}
	double adouble;
};

class derived : public base
{
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int file_version){
		boost::serialization::void_cast_register<derived, base>(static_cast<derived *>(NULL),static_cast<base *>(NULL));
		ar & BOOST_SERIALIZATION_NVP(aint);
	}
public:
	derived(int a, double b) : base(b), aint(a) {}
	int aint;
};

BOOST_CLASS_EXPORT_GUID(derived, "derived")

class manager
{
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int file_version){
		ar & BOOST_SERIALIZATION_NVP(objs);
	}
public:
	void save(const std::string);
	void load();

	std::list<base*> objs;
};

void SerializeTest::setUp()
{

}

void SerializeTest::tearDown()
{

}

void SerializeTest::testSerialize()
{
	LOG4CXX_INFO(logger, "Running " << __FUNCTION__);

	manager mgr;

	// Creat some data to save
	mgr.objs.push_back( derived(1, 1.0) );
	mgr.objs.push_back( derived(2, 3.0) );
	mgr.objs.push_back( derived(3, 5.0) );
	mgr.objs.push_back( derived(4, 7.0) );

	std::string filename("out.xml");

	{
		std::ofstream ofs(filename);
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(mgr);
	}

	manager new_mgr;

	{
		std::ifstream ifs(filename);
		assert(ifs.good());
		boost::archive::xml_iarchive ia(ifs);
		// restore the schedule from the archive
		ia >> BOOST_SERIALIZATION_NVP(new_mgr);
	}

	filename = "check.xml";
	{
		std::ofstream ofs(filename);
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(new_mgr);
	}

	LOG4CXX_INFO(logger, "Exiting " << __FUNCTION__);
}


