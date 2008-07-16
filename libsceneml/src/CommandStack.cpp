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
 * CommandStack.cpp
 *
 *  Created on: Jul 16, 2008
 *      Author: yamokosk
 */

#include "CommandStack.h"

namespace sml
{

CommandStack::CommandStack()
{
	// TODO Auto-generated constructor stub

}

CommandStack::~CommandStack()
{
	// TODO Auto-generated destructor stub
}

CommandStack::add(CommandBase* c)
{
	commands.push_back(c);
}

CommandStack::undo(void)
{
	if (commands.size() > 1) {
		commands.pop_back();
	} else {
		throw std::runtime_error("No message.")
	}
}

CommandStack::process(void)
{
	for (std::vector<Command*>::size_type x=0; x < commands.size(); ++x) {
		commands[x]->execute();
	}
}

}
