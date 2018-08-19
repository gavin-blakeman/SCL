//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								error
// SUBSYSTEM:						Error Definitions
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	GCL.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
//                      This file is part of the Storage Class Library (PCL)
//
//                      SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or
//                      (at your option) any later version.
//
//                      SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with SCL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
//
// OVERVIEW:						Common definitions that can be used by multiple files
//
//
// CLASSES INCLUDED:		None
//
// CLASS HIERARCHY:     None.
//
// HISTORY:             2018-08-18 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef SCL_ERROR_H
#define SCL_ERROR_H

  // Standard libraries

#include <cstdint>
#include <string>

  // Miscellaneous libraries

#include "include/Error.h"      //!!! If the full GCL package is included at this point, it creates cicular references and errors.

namespace SCL
{

#define SCL_ERROR(ERR) (ERROR(SCL, ERR))
#define SCL_CODE_ERROR CODE_ERROR(SCL)

}

#endif // SCL_COMMON_H
