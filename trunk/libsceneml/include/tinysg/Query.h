/*************************************************************************
* TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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
 * Query.h
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#ifndef QUERY_H_
#define QUERY_H_

#include <string>

namespace TinySG {

// Forward declarations
class ObjectManager;
class Query;
class PropertyCollection;

class QueryResult
{
public:
	QueryResult(Query* q) : query_(q) {};
	virtual ~QueryResult() {};

	Query* getCreator() {return query_;}

protected:
	Query* query_;
};


class Query
{
public:
	Query(const std::string& type) : type_(type) {};
	virtual ~Query() {};
	virtual QueryResult* execute(const ObjectManager* mgr, const PropertyCollection* params = 0) = 0;
	virtual const std::string& getType() {return type_;}
private:
	std::string type_;
};

}

#endif /* SCENEQUERY_H_ */
