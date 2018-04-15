//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Vector
// SUBSYSTEM:           Array Implementation
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2010, 2013-2017 Gavin Blakeman.
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
// OVERVIEW:            1) Vectors only allow insertion of packages at the end
//                      2) Vectors allow random retrieval of packages
//                      3) Vectors will automatically resize as required
//                      4) Items can be inserted/deleted in the middle of the Vector.
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2010-04-28 GGB - Unscrabble Application Release 1.00
//*********************************************************************************************************************************

#include "../Include/Vector.h"

#include <stdio.h>

namespace SCL
{
#define VECTOR_INIT_SIZE	1024
#define VECTOR_INCR_SIZE	1024
#define VECTOR_INFLATOR		2

  //***********************************************************************************************
  //
  // CVector
  //
  // Vector class implementation code
  //
  //***********************************************************************************************

  // Default class constructor.
  // Initialise all variables and allocate the default sized array.
  //
  // 2010-04-21/GGB - Function created.

  CVector::CVector() : CContainer()
  {
    lInitialSize = VECTOR_INIT_SIZE;
    vectorIncrementSize = VECTOR_INCR_SIZE;
    PackageArray = new PPackage[lInitialSize];
    if (PackageArray)
      currentVectorSize = lInitialSize;
    else currentVectorSize = 0;

    for (vectorIndex = 0; vectorIndex < currentVectorSize; vectorIndex++)
      PackageArray[vectorIndex] = NULL;

    vectorIndex = -1;					// Next insertion will be at 0.
    vectorIncrements = 0;
    inflatorValue = VECTOR_INFLATOR;
  }

  // Class constructor to specify initial size and increment size.
  // If InitialSize and IncrementSize are less than zero, then the
  // default values of initial size and increment size are used.
  //
  // 2010-04-21/GGB - Function created.

  CVector::CVector(long lnewInitialSize, long lnewIncrementSize)
  {
    if (lnewInitialSize > 0)
      lInitialSize = lnewInitialSize;
    else lInitialSize = VECTOR_INIT_SIZE;

    if (lnewIncrementSize > 0)
      vectorIncrementSize = lnewIncrementSize;
    else vectorIncrementSize = VECTOR_INCR_SIZE;

    PackageArray = new PPackage[lInitialSize];
    if (PackageArray)
      currentVectorSize = lInitialSize;
    else currentVectorSize = 0;

    for (vectorIndex = 0; vectorIndex < currentVectorSize; vectorIndex++)
      PackageArray[vectorIndex] = NULL;

    vectorIndex = -1;
    vectorIncrements = 0;
    inflatorValue = VECTOR_INFLATOR;
  }

  // Default destructor for the class. Ensures that all dynamically allocated memory is freed
  // up before exiting.
  // If the objects are owned then the objects are also deleted at this stage.
  //
  // 2010-04-21/GGB - Function created.

  CVector::~CVector()
  {
    long nIndex;

    if (ownsPackages())
    {
      for (nIndex = 0; nIndex < vectorIndex; nIndex++)
      {
        if (PackageArray[nIndex])
        {
          delete PackageArray[nIndex];
          PackageArray[nIndex] = nullptr;
        };
      };
    };
    delete [] PackageArray;
    PackageArray = nullptr;
    vectorIndex = -1;
  }

  // Performs the array lookup function.
  /// EXCEPTIONS: 0x0003 - Null package
  //
  // 2015-05-21/GGB - Changed NULL_PACKAGE to a throw.
  // 2010-04-25/GGB - Function created.

  CPackage &CVector::operator[](long lIndex)
  {
    if (lIndex <= vectorIndex)
    {
      return *PackageArray[lIndex];
    }
    else
    {
      SCL_ERROR(0x0003);
    }
  }

  // Inserts an item at the end of the vector.
  //
  // 2010-04-25/GGB - Function created.

  bool CVector::Insert(CPackage *toInsert)
  {
      // first check vector bounds.

    if (vectorIndex < (currentVectorSize-1))
    {	// Can just insert new item.
      vectorIndex++;
      PackageArray[vectorIndex] = toInsert;
      incPackageCount();		// Increase the package count.
    }
    else
    {	// Need to resize the vector.
      resizeVector();			// Adds new space into the vector.

      vectorIndex++;
      PackageArray[vectorIndex] = toInsert;
      incPackageCount();
    };
    return true;
  }

  // Resizes the vector. Increases the size by vectorIncrementSize
  //
  // 2010-04-25/GGB - Function created.

  bool CVector::resizeVector()
  {
    CPackage **newPackageArray;
    long lIndex;

    newPackageArray = new PPackage[currentVectorSize+vectorIncrementSize];

    for (lIndex = 0; lIndex < currentVectorSize; lIndex++)
    {
      newPackageArray[lIndex] = PackageArray[lIndex];		// Copy the item into the new array.
      PackageArray[lIndex] = nullptr;											// ensure delete does not delete packages.
    };

    delete [] PackageArray;
    PackageArray = newPackageArray;
    newPackageArray = nullptr;
    currentVectorSize += vectorIncrementSize;
    vectorIncrements++;
    if (!(vectorIncrements%inflatorValue))
    {
      vectorIncrementSize *= 2;											// Vector inflator size to double as large
                                                    // number of increments happening.
      vectorIncrements = 0;
    };
    return true;
  }

  // Sets the new inflatorValue
  //
  // 2010-04-25/GGB - Function created.

  void CVector::SetInflatorValue(int newValue)
  {
    if (inflatorValue > 2)
      inflatorValue = newValue;
  }

  // Removes an item from the vector.
  //
  // 2010-04-25/GGB - Function created.

  bool CVector::Remove(CPackage *toRemove)
  {
    long lIndex = 0;

      // find the package in the array.

    while ( (lIndex <= vectorIndex) && (PackageArray[lIndex] != toRemove) )
      ++lIndex;

    if (lIndex > vectorIndex)
    {	// Item not found
      return false;			// Item could not be found.
    }
    else
    {	// Found the item. Remove the item and delete if required.
      if (ownsPackages())
        delete PackageArray[lIndex];
      PackageArray[lIndex] = NULL;

        // Copy the remainder of the items back along the vector.

      for (; lIndex <= vectorIndex; lIndex++)
      {
        PackageArray[lIndex] = PackageArray[lIndex+1];
        ++lIndex;
      };
      --vectorIndex;
      return true;
    };
  }

  /// @brief Empties the vector.
  /// @throws None.
  /// @version 2010-04-25/GGB - Function created.

  void CVector::Empty()
  {
    long lIndex;

    for (lIndex = 0; lIndex <= vectorIndex; lIndex++)
    {
      if (ownsPackages())
      {
        delete PackageArray[lIndex];
      };
      PackageArray[lIndex] = nullptr;
    };
    vectorIndex = -1;
  }

  //************************************************************************************************
  //
  // CVectorIterator
  //
  //************************************************************************************************
}
