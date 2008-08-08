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
 * Variable.cpp
 *
 *  Created on: Jul 16, 2008
 *      Author: yamokosk
 */

#include "Variable.h"

namespace tinysg
{

Variable::Variable() :
	Subject()
{
	pc_.addPair( RequiredProperty("type") );
	pc_.addPair( RequiredProperty("subtype") );
}

Variable::~Variable()
{
}

void Variable::setType(const std::string& type)
{
	pc_.updatePair("type", type.c_str(), true);
}

std::string Variable::getType()
{
	return pc_.getValue("type");
}

void Variable::setSubType(const std::string& subtype)
{
	pc_.updatePair("subtype", subtype.c_str(), true);
}

std::string Variable::getSubType()
{
	return pc_.getValue("subtype");
}

void Variable::setScalar(Real s)
{
	data_.resize(1);
	data_(1) = s;
	this->notify(ScalarUpdate);
}

Real Variable::getScalar()
{
	return data_(1);
}

void Variable::setVector(const ColumnVector& v)
{
	data_.cleanup();
	data_ = v;
	this->notify(VectorUpdate);
}

ReturnMatrix Variable::getVector()
{
	ColumnVector ret(data_);
	ret.Release();
	return ret;
}

}
