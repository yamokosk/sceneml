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

#ifndef SUBJECT_H
#define SUBJECT_H

#include <boost/signal.hpp>
#include <boost/bind.hpp>

namespace sml {

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

};

#endif
