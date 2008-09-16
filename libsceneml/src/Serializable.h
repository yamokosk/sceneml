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
/*
 * Serialize.h
 *
 *  Created on: Sep 15, 2008
 *      Author: yamokosk
 */

#ifndef SERIALIZE_H_
#define SERIALIZE_H_

#include <string>

namespace TinySG
{

class Serializable
{
protected:
	template
	void save(Archive & ar, const unsigned int version) const
	    {
	        // note, version is always the latest when saving
	        ar  & driver_name;
	        ar  & stops;
	    }
	    template<class Archive>
	    void load(Archive & ar, const unsigned int version)
	    {
	        if(version > 0)
	            ar & driver_name;
	        ar  & stops;
	    }
};

}

#endif /* SERIALIZE_H_ */
