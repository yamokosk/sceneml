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
 * ObjectModel.h
 *
 *  Created on: Nov 21, 2008
 *      Author: yamokosk
 */

#ifndef OBJECTMODEL_H_
#define OBJECTMODEL_H_

#include <string>
#include <map>

typedef float Real;

/*
 * Stores name value pairs
 */
struct PropertyPair
{
	std::string name;
	std::string value;
	bool optional;
};

/*
 * Stores many property pairs
 */
struct PropertyCollection
{
	std::map<std::string, PropertyPair> properties;
};


/*
 * Used to store the result of a scene query.
 */
struct IQueryResult {};
struct QueryInfo {};

/*
 * Query interface
 */
struct IQuery
{
	virtual void getInfo(QueryInfo* info) = 0;
	virtual void execute(IQueryResult* result, const PropertyCollection* params = 0) = 0;
};

/*
 * Scene object info
 */
struct IObjectInfo {};
struct IWriteBuffer {};
struct IReadBuffer {};

/*
 * Scene object interface
 */
struct ISceneObject
{
	virtual void init(IObjectInfo& objectInfo) = 0;
	//virtual void execute(IReadBufferIterator & args, IWriteBuffer & out) = 0;
	virtual void getParameter(const Byte * name, IWriteBuffer & value) = 0;
	virtual void setParameter(const Byte * name, IReadBuffer & value) = 0;
	virtual void saveState(IWriteBuffer & state) = 0;


	virtual void getInfo( IObjectInfo* info ) = 0;
	virtual void notifyMoved( const Real* translation, const Real* rotation ) = 0;
};



#endif /* OBJECTMODEL_H_ */
