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
 * Serializer.h
 *
 *  Created on: Sep 15, 2008
 *      Author: yamokosk
 */

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include "Serializable.h"

#include <tinyxml/tinyxml.h>

// Logging
#include <log4cxx/logger.h>

namespace TinySG
{

class Serializer
{
	static log4cxx::LoggerPtr logger;

public:
	Serializer();
	virtual ~Serializer();

	void save(const std::string& filename);

	// a single item
	bool serialize(Serializable*);
	//int	deserialize(ObjectFactory*, CPtrList& objList);

private:
	void serializeObject(Serializable* obj, TiXmlElement* parent);
	//int deserializeObject(ObjectFactory*, CPtrList& objList,MSXML::IXMLDOMNode* parent=NULL);

	TiXmlElement* root_;
};

}

#endif /* SERIALIZER_H_ */
