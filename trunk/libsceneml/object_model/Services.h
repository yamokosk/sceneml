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
 * Services.h
 *
 *  Created on: Nov 21, 2008
 *      Author: yamokosk
 */

#ifndef SERVICES_H_
#define SERVICES_H_

typedef struct LogParams
{
	const char* filename;
	unsigned int line;
	const char* message;
} LogParams;

typedef struct ReportErrorParams
{
	const char* filename;
	unsigned int line;
	const char* message;
} ReportErrorParams;


#endif /* TINYSGSERVICES_H_ */
