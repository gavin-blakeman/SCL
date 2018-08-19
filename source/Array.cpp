//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Array
// SUBSYSTEM:           Array Implementation
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2010-2018 Gavin Blakeman.
//                      This file is part of the Storage Class Library (SCL)
//
//                      SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with SCL.  If not, see
//                      <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            1) Arrays allow insertion of objects in any position in the array as specified by the Index
//                      2) Arrays allow retrieval of data from any position in the array as specified by Index
//                      3) Arrays can be iterated forwards and backwards
//                      4) Un-assigned values in the array will be returned as NULL_PACKAGE
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//
//*********************************************************************************************************************************//*************************************************************************************************

#include "../include/Array.h"

#include <cstdio>

namespace SCL
{
#define ARRAY_INIT_SIZE	100
#define ARRAY_INCR_SIZE	100

  //************************************************************************************************
  //
  // CArray
  //
  // Array Storage Class.
  // Stores Packages in a continuous array.
  //
  //************************************************************************************************

  // Default contructor for the array.
  // The initial size and initial increment are set to the values ARRAY_INIT_SIZE and
  // ARRAY_INCR_SIZE defined in this file.
  // To change the initial size and increment size, either use the constructor that takes the
  // initial size and increment, or call the function to change the values.
  //
  // 2010-04-20/GGB - Function Created

  CArray::CArray()
  {
    lInitialSize = ARRAY_INIT_SIZE;
    lIncrementSize = ARRAY_INCR_SIZE;
    lCurrentSize = 0;
    PackageArray = new PPackage[lInitialSize];
  }

  // Deletes all memory allocated by and owned by the array class.
  //
  // 2010-04-20/GGB - Function created

  CArray::~CArray(void)
  {
    if (PackageArray)
    {
      delete[] PackageArray;
      PackageArray = nullptr;
    };
  }

  /// Returns the referenced item in the array.
  /// EXCEPTIONS:  0x0003
  ///              0x0100
  //
  // 2015-05-21/GGB - Removed NULL_PACKAGE and have function throw instead.
  // 2010-04-20/GGB - Function created

  CPackage &CArray::operator[](long lIndex)
  {
    if (lIndex >= lCurrentSize)
    {
      SCL_ERROR(0x0100);      // CArray: Index out of bounds
    }
    else
    {
      if (PackageArray[lIndex])
      {
        return ( *PackageArray[lIndex] );
      }
      else
      {
        SCL_ERROR(0x0003);      // Null package
      }
    };
  }

  bool CArray::Insert(CPackage *)
  {
    return false;
  }

  bool CArray::Remove(CPackage *)
  {
    return false;
  }

  void CArray::Empty(void)
  {
  }

} // namespace SCL
