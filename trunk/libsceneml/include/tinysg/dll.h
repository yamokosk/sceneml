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
 * Code taken from the Orca-Robotics Project
 * http://orca-robotics.sf.net/
 */

#ifndef DYNAMICLOAD_DLL_H
#define DYNAMICLOAD_DLL_H

#include <string>

namespace TinySG
{

//!
//! Class to encapsulate a dynamically-loaded library, so you can load
//! code at run-time.
//!
//! Note that you have to keep this thing in scope for as long as you
//! want to access code from it.
//!
class DynamicallyLoadedLibrary
{
public:

    //! Loads the lib, throws DynamicLoadExceptions on error.
    DynamicallyLoadedLibrary( const std::string &libName );
    ~DynamicallyLoadedLibrary();

    //! Library name
    const std::string &name() const { return libName_; }

    //! Library handle
    void *handle() { return libHandle_; }

private:

    void        *libHandle_;
    std::string  libName_;
};

} // namespace

#endif
