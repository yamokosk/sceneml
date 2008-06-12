/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License, 
 * or (at your option) any later version. The text of the GNU Lesser General 
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for 
 * more details.
 *
 *************************************************************************/
 
#ifndef _SCENEML_SIGNALS_H_FILE_
#define _SCENEML_SIGNALS_H_FILE_

#include <boost/signal.hpp>
#include <boost/bind.hpp>

namespace sceneml {

class Subject
{
public:
	typedef boost::signal<void (bool)>  signal_t;
	typedef boost::signals::connection  connection_t;

public:
	Subject() {};
	
	connection_t connect(signal_t::slot_function_type subscriber)
	{
		return sig_.connect(subscriber);
	}

	void disconnect(connection_t subscriber)
	{
		subscriber.disconnect();
	}

	virtual void notify()
	{
		sig_(true);
	}

private:
    signal_t sig_;
};


class Listener
{
public:
	Listener(Subject& m) : subject_(m)
	{
		connection_ = subject_.connect( boost::bind(&Observer::notify, this, _1) );
    }

    virtual ~Listener()
	{
		subject_.disconnect(connection_);
	}

	virtual void notify(bool bExtended) const = 0;

protected:
	Subject& subject_;

private:
	Subject::connection_t  connection_;
};

};

#endif