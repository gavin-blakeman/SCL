//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Vector
// SUBSYSTEM:           Array Implementation
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2010, 2013-2018 Gavin Blakeman.
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
//
//*********************************************************************************************************************************

#ifndef SCL_VECTOR_H
#define SCL_VECTOR_H

#include "Base.h"

namespace SCL
{
  class CVector : public CContainer
  {
  private:
    CPackage **PackageArray;
    long lInitialSize;											// Initial size of the array (Number of items)
    long vectorIncrementSize;								// Amount to increment the array (Number of items)
    long currentVectorSize;									// Current Size of the Array (Number of items)
    long vectorIndex;												// Current insertion point. (Starts at -1)
    int vectorIncrements;										// Number of increments performed.
    int inflatorValue;											// Number of increments before inflation

  protected:
    bool resizeVector();					// Resizes the vector -> Always bigger.
    bool inflateVector();					// Increases the size of the vector
    bool deflateVector();					// Decreases the size of the vector

  public:
    CVector();
    CVector(long, long);
    virtual ~CVector(void);

    virtual bool Insert(CPackage *);  // Insert an object into the container.
    virtual bool Remove(CPackage *);
    virtual void Empty();

    inline virtual void SetInflatorValue(int);

    CPackage &operator[](long);
  };

  class CVectorIterator : public CIterator
  {
  private:
  protected:
  public:
    CVectorIterator(CVector *);

    virtual void MoveFirst();
    virtual void MoveLast();
    virtual void MoveNext();
    virtual void MovePrevious();
  };
}

#endif // SCL_VECTOR_H
