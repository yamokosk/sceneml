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
 * Variable.h
 *
 *  Created on: Jul 16, 2008
 *      Author: yamokosk
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "PropertyCollection.h"
#include "Subject.h"

namespace TinySG
{

class Variable : public Subject
{
public:
	enum VariableHint
	{
		ScalarUpdate=100,
		VectorUpdate
	};

	Variable();
	virtual ~Variable();

	void setType(const std::string&);
	std::string getType();

	void setSubType(const std::string&);
	std::string getSubType();

	void setScalar(Real s);
	Real getScalar();

	void setVector(const ColumnVector& v);
	ReturnMatrix getVector();

protected:
	PropertyCollection pc_;
	ColumnVector data_;
};

}

#endif /* VARIABLE_H_ */
