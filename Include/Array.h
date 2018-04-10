//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Array
// SUBSYSTEM:           Array Implementation
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2010-2016 Gavin Blakeman.
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
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//
//*********************************************************************************************************************************

#ifndef SCL_ARRAY_H
#define SCL_ARRAY_H

#include "Base.h"

namespace SCL
{
  class CArray : public CContainer
  {
  private:
    CPackage **PackageArray;
    long lInitialSize;											// Initial size of the array (Number of items)
    long lIncrementSize;										// Amount to increment the array (Number of items)
    long lCurrentSize;											// Current Size of the Array (Number of items)

  protected:
  public:
    CArray(void);
    CArray(long);
    virtual ~CArray(void);

    virtual bool Insert(CPackage *);  // Insert an object into the container.
    virtual bool Remove(CPackage *);
    virtual void Empty(void);

    CPackage &operator[](long);
  };

  class CArrayIterator : public CIterator
  {
  private:
  protected:
  public:
    CArrayIterator(CArray *);
  };
}

#endif // SCL_ARRAY_H
