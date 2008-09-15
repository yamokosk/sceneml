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

#include "PropertyCollection.h"

#include <string>
#include <vector>

namespace TinySG
{

class Serializable
{
public:
	typedef std::vector<Serializable*> ComplexProperties;

	virtual void getAttributes(PropertyCollection& pc) = 0;
	virtual ComplexProperties getProperties() = 0;
	virtual std::string getClassName() = 0;
};

class ObjectFactory
{
public:
	virtual Serializable* create() = 0;
	void destroy(Serializable* obj) {delete obj;}
};

}

#endif /* SERIALIZE_H_ */
