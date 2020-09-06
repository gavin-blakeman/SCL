//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								error
// SUBSYSTEM:						Error Definitions
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018-2020 Gavin Blakeman.
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

#include "config.h"

#ifdef USE_GCL
#include "../GCL/include/error.h" //!!! If the full GCL package is included at this point, it creates cicular references and errors.
#else
#include <stdexcept>
#endif

namespace SCL
{
#ifdef USE_GCL
  #define SCL_ERROR(errorNo) (ERROR(SCL, errorNo))
#else
#define SCL_ERROR(errorNo) {throw std::runtime_error(std::to_string((errorNo))); }
#endif
}

#endif // SCL_COMMON_H
