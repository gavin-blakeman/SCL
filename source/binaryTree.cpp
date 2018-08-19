//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                BTree.h
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
// OVERVIEW:            1. Items are inserted into the binary tree in the correct sorted position
//                      2. The binary tree iterator will iterate the tree in the correct order. (Asc and Desc)
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2015-04-02 GGB - Removed all deprecated functions.
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2010-04-28/GGB - Unscrabble Application Release 1.00
//                      2010-04-22/GGB - File Created.
//
//*********************************************************************************************************************************

#include "../Include/binaryTree.h"
#include "../Include/Vector.h"

#include "../Include/algorithm.hpp"

#include <new>
#include <cstdio>

namespace SCL
{

  //************************************************************************************************
  //
  // CBTreeIterator
  //
  // All iterations of the BTree are either done InOrder (smallest to Largest) or Largest to
  // smallest.
  //
  //************************************************************************************************




  // Returns the first value in the list.
  //
  // 2010-04-22/GGB - Function created.

  CPackage *CBTreeIterator::GetFirst()
  {
    if (BOC())
      return ( Current->Data );
    else
    {
      MoveFirst();
      return ( Current->Data );
    };
  }

  // Moves to the last item on the tree. (RHS)
  //
  // 2010-04-22/GGB - Function created.

  CPackage *CBTreeIterator::GetLast()
  {
    if (EOC())
      return ( Current->Data );
    else
    {
      MoveLast();
      return ( Current->Data );
    };
  }

  // Gets the next item in the BTree.
  //
  // 2010-04-25/GGB - Function created.

  CPackage *CBTreeIterator::GetNext(void)
  {
    MoveNext();
    if (Current)
      return ( Current->Data );
    else
      return nullptr;
  }

  // Gets the previous package in the BTree.
  //
  // 2010-04-25/GGB - Function created.

  CPackage *CBTreeIterator::GetPrevious()
  {
    MovePrevious();
    if (Current)
      return ( Current->Data );
    else
      return NULL;
  }

  /// Gets the current item from the tree.
  //
  // 201-004-25/GGB - Function created.

  CPackage *CBTreeIterator::GetCurrent()
  {
    if (Current)
      return ( Current->Data );
    else
      return nullptr;
  }

}   // SCL
