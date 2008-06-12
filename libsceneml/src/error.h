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

#ifndef _SML_ERROR_H_
#define _SML_ERROR_H_

#include <string>
#include <sstream>
#include "errorcodes.h"


///////////////////////////////////////////////////////////////////////////////
// SMLError
// --------
// Micro-Manager error class, used to create exception objects
// 
class SMLError
{
public:
   SMLError(const char* msg, int code) :
      specificMsg_(msg),
      errCode_(code) {}

   SMLError(const char* specificMsg, const char* coreMsg, int code) :
      specificMsg_(specificMsg),
      coreMsg_(coreMsg),
      errCode_(code) {}

   SMLError(int code) :
      errCode_(code) {}

   virtual ~SMLError() {}

   virtual std::string getMsg()
   {
      std::ostringstream msg;
      msg << "Error code: " << errCode_ << std::endl;
      if (!specificMsg_.empty())
         msg << specificMsg_ << std::endl;
      msg << coreMsg_;
      return msg.str();
   }
   int getCode() {return errCode_;}
   void setCoreMsg(const char* msg) {coreMsg_ = msg;}

private:
   std::string coreMsg_;
   std::string specificMsg_;
   long errCode_;
};

#endif //_ERROR_H_
