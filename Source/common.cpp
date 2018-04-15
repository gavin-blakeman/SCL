//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								common
// SUBSYSTEM:						Common definitions
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017 Gavin Blakeman.
//                      This file is part of the Storage Class Library (SCL)
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
// FUNCTIONS INCLUDED:  std::string getVersionString()
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-06-29 GGB - File created.
//
//*********************************************************************************************************************************

#include "../Include/common.h"

  // Standard libraries

#include <iomanip>
#include <sstream>
#include <string>

  // Boost Library

#include <boost/algorithm/string.hpp>

namespace SCL
{

  unsigned int const SCL_MAJORVERSION = 2017;           ///< The Major version number (yyyy)
  unsigned int const SCL_MINORVERSION = 9;              ///< The Minor version number (mm)
  unsigned int const SCL_BUILDNUMBER  = 0x0000;         ///< The build numnber - incremented on each build.

  /// @brief Returns the library version and build number that was used for editing the library.
  /// @returns A string with the application name, version, build.
  /// @throws Nothing.
  /// @version 2015-06-29/GGB - Function created.

  std::string getVersionString()
  {
    std::stringstream s;

    s << SCL_MAJORVERSION << "." << std::setfill('0') << std::setw(2) << SCL_MINORVERSION << std::setw(4) << std::hex << SCL_BUILDNUMBER << std::endl;

    std::string returnValue = s.str();
    boost::algorithm::to_upper(returnValue);

    return returnValue;
  }

  void loadErrorMessage()
  {
    std::vector<std::pair<GCL::TErrorCode, std::string>> errors =
    {
      {0x0001, std::string("Index out of bounds")},
      {0x0002, std::string("Variable not initialised") },
      {0x0003, std::string("Null Package") },
      {0x0004, std::string("Container is empty.") },
      {0x0100, std::string("Array: Index out of bounds") },
      {0x0200, std::string("CAny: PackageDate == nullptr") },
      {0x0300, std::string("ARRAY2D: stride value cannot be zero.") },
      {0x0301, std::string("ARRAY2D: Dimensions cannot be zero.") },
      {0x0302, std::string("ARRAY2D: Index out of bounds")},
      {0x0400, std::string("ARRAY1DP: Arrays are not the same dimensions.")},
      {0x1000, std::string("AVLTree: Index out of Bounds") },
      {0x2000, std::string("VectorSorted: Incorrect iterator for object.")},
      {0x2001, std::string("VectorSorted: Index out of bounds [0; size()].")}
    };

    std::for_each(errors.begin(), errors.end(),
                  [] (std::pair<GCL::TErrorCode, std::string> p) { GCL::CError::addErrorMessage("SCL", p.first, p.second); });
  }
}
