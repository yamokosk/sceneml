/*
 * CommandBase.h
 *
 *  Created on: Jul 16, 2008
 *      Author: yamokosk
 */

#ifndef COMMANDBASE_H_
#define COMMANDBASE_H_

#include "PropertyCollection.h"

namespace sml {


class CommandBase : public PropertyCollection
{
public:
	CommandBase();
	virtual ~CommandBase();

	virtual void execute() = 0;
};

}

#endif /* COMMANDBASE_H_ */
