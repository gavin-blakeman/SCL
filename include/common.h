//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								common
// SUBSYSTEM:						Common definitions
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	GCL.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2019 Gavin Blakeman.
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
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-06-29 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef SCL_COMMON_H
#define SCL_COMMON_H

  // Standard libraries

#include <cstdint>
#include <string>

namespace SCL
{

  typedef std::uint8_t base_t;

  std::string const SCL_LIBNAME       = "SCL";

  std::string getVersionString();
}

#endif // SCL_COMMON_H
