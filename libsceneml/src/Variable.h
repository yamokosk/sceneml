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

namespace sml
{

class Variable : public PropertyCollection, public Subject
{
public:
	Variable();
	virtual ~Variable();
};

}

#endif /* VARIABLE_H_ */
