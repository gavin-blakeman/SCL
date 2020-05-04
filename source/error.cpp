//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								error
// SUBSYSTEM:						Error definitions
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018-2020 Gavin Blakeman.
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
// CLASSES INCLUDED:		CLoader - Static instance created to load messages during application start.
//
// CLASS HIERARCHY:     None.
//
// FUNCTIONS INCLUDED:
//
// HISTORY:             2018-08-18 GGB - File Created
//
//*********************************************************************************************************************************

#include "../include/error.h"

  // Standard C++ library header files.

#include <string>
#include <vector>

#ifdef USE_GCL

#include <GCL>

namespace SCL
{

  class CLoader
  {
  private:
    void loadErrorMessages();

  public:
    CLoader();
  };

  static CLoader loader;

  CLoader::CLoader()
  {
    loadErrorMessages();
  }

  void CLoader::loadErrorMessages()
  {
    std::vector<std::pair<GCL::TErrorCode, std::string>> errors =
    {
      {0x0001, "Index out of bounds"},
      {0x0002, "Variable not initialised"},
      {0x0003, "Null Package"},
      {0x0004, "Container is empty." },
      {0x0100, "Array: Index out of bounds"},
      {0x0200, "CAny: PackageDate == nullptr"},
      {0x0300, "ARRAY2D: stride value cannot be zero."},
      {0x0301, "ARRAY2D: Dimensions cannot be zero."},
      {0x0302, "ARRAY2D: Index out of bounds"},
      {0x0400, "ARRAY1DP: Arrays are not the same dimensions."},
      {0x1000, "AVLTree: Index out of Bounds"},
      {0x2000, "VectorSorted: Incorrect iterator for object."},
      {0x2001, "VectorSorted: Index out of bounds [0; size()]."}
    };

    std::for_each(errors.begin(), errors.end(),
                  [] (std::pair<GCL::TErrorCode, std::string> p) { GCL::CError::addErrorMessage("SCL", p.first, p.second); });
  }
}

#endif
