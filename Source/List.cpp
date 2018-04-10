//*****************************************************************************
//
// PROJECT:			Storage Class Library (SCL)
// FILE:				List.h
// SUBSYSTEM:		Linked List Implementation
// TARGET OS:		All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
// Copyright 2010, 2013 Gavin Blakeman.
// This file is part of the Storage Class Library (SCL)
//
// SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
//
// SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with SCL.  If not, see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:
// 1) List always inserts items at the end of the list.
// 2) Items can be deleted randomly in the list.
// 3) The list can be traversed in either direction.
// 4) Implemented using a doubly-linked list.
//
// HISTORY:             Release 1.00	- 28 April 2010					=	Unscrabble Application Release 1.00
//
//*****************************************************************************

#include "../Include/List.h"

#include <stdio.h>

namespace SCL
{
  //************************************************************************************************
  //
  // SListNode
  //
  // Used for linking the list to the data.
  //
  //************************************************************************************************

  struct SListNode
  {
    CPackage *Data;
    SListNode *Previous;
    SListNode *Next;
  };

  //************************************************************************************************
  //
  // CList
  //
  // Linked List Class
  //
  //************************************************************************************************


  // Default constructor for a list object.
  //
  // Revision:		1.00
  // Date:				20 April 2010
  // Written by:	Gavin Blakeman

  CList::CList(void) : CContainer()
  {
    Head = Tail = NULL;
  };

  // Default destructor for a list object.
  //
  // Revision:		1.00
  // Date:				28 April 2010
  // Written by:	Gavin Blakeman

  CList::~CList(void)
  {
    Empty();
  };

  // Inserts an item into the linked list.
  // The item is always inserted at the end of the list. (Tail)
  //
  // Revision:		1.00
  // Date:				20 April 2010
  // Written by:	Gavin Blakeman

  bool CList::Insert(CPackage *newPackage)
  {
    SListNode *newNode = new SListNode;

    newNode->Data = newPackage;
    newNode->Next = NULL;
    newNode->Previous = Tail;

    if (!Head)					// First insertion into the list.
      Head = newNode;

      // Now insert the package at the end of the list.
    if (Tail)
      Tail->Next = newNode;		// Existing list

    Tail = newNode;

    newNode = NULL;
    incPackageCount();
    return true;
  };

  bool CList::Remove(CPackage *)
  {
    return true;
  };

  // Empties the list. The fate of the packages is determined by the OwnsPackages() function
  // derived from CContainer.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  void CList::Empty(void)
  {
    SListNode *Current, *Last;

    Current = Head;

    while (Current)
    {
      if (ownsPackages())
      {
        delete Current->Data;
        decPackageCount();
      };
      Last = Current;
      Current->Data = NULL;
      Current->Previous = NULL;
      Current = Current->Next;
      Last->Next = NULL;
      delete Last;
    };
    Current = NULL;
    Head = NULL;
    Tail = NULL;
    resetPackageCount();
  };

  //************************************************************************************************
  //
  // CListIterator
  //
  // Code for the list iterator class.
  //
  //************************************************************************************************

  // Default constructor for the CListIterator class.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  CListIterator::CListIterator(CList *newList) : CIterator()
  {
    List = newList;
    lCurrenINDEX_t = -1;
  };

  // Deletes any dynamically allocated memory.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  CListIterator::~CListIterator(void)
  {

  };

  // Moves to the first item in the list. If the list is empty, returns a pointer to the
  // PACKAGE_NULL object, otherwise points to the first item in the list.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  CPackage *CListIterator::GetFirst(void)
  {
    Current = List->Head;
    if (Current)
      return Current->Data;
    else return NULL;
  };

  // Moves to the last item in the list. If the list is empty, returns NULL, otherwise returs
  // a pointer to the item
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  CPackage *CListIterator::GetLast(void)
  {
    Current = List->Tail;
    if (Current)
      return Current->Data;
    else return NULL;
  };

  // Moves to the next item in the list and returns it. If the item is empty, returns the null
  // value and sets EOF.
  // If the iterator is not pointing at the list yet, it will point to and return the first
  // package in the list.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  CPackage *CListIterator::GetNext(void)
  {
    CPackage *retVal;

    if (Current)
    {
      Current = Current->Next;
      if (Current)
      {
        ResetEOC();
        retVal = Current->Data;
      }
      else
      {
        SetEOC();
        retVal = NULL;
      };
    }
    else
    {
      Current = List->Head;
      if (Current)
      {
        ResetEOC();
        retVal = Current->Data;
      }
      else
      {
        SetEOC();
        retVal = NULL;
      };
    };

    return ( retVal );
  };

  // Moves to the previous item in the list.
  // If there is no item currently pointed to, moves to the end of the list and returns the
  // end of list value or null.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  CPackage *CListIterator::GetPrevious(void)
  {
    PPackage retVal;

    if (Current)
    {
      Current = Current->Previous;
      if (Current)
      {
        retVal = Current->Data;
        ResetBOC();
      }
      else
      {
        SetBOC();
        retVal = NULL;
      }
    }
    else
    {
      Current = List->Tail;
      if (Current)
      {
        ResetBOC();
        retVal = Current->Data;
      }
      else
      {
        SetBOC();
        retVal = NULL;
      };
    };

    return ( retVal );
  };

  // Returns the package currently pointed at by the Iterator.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  CPackage *CListIterator::GetCurrent(void)
  {
    CPackage *retVal = NULL;

    if (Current)
      retVal = Current->Data;
    else
      retVal = NULL;

    return ( retVal );
  };

  // Moves to the first item in the list.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  void CListIterator::MoveFirst(void)
  {
    Current = List->Head;
    if (Current)
    {
      ResetBOC();
      ResetEOC();
    }
    else
    {
      SetBOC();
      ResetEOC();
    };
  };

  // Moves to the last item in the list.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  void CListIterator::MoveLast(void)
  {
    Current = List->Tail;
    if (Current)
      ResetEOC();
    else SetEOC();
  };

  // Moves to the next item in the list.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  void CListIterator::MoveNext(void)
  {
    if (!EOC())
    {
      Current = Current->Next;
      if (Current)
        ResetEOC();
      else
        SetEOC();
    };
  };

  // Moves to the previous item in the list.
  //
  // Revision:		1.00
  // Date:				21 April 2010
  // Written by:	Gavin Blakeman

  void CListIterator::MovePrevious(void)
  {
    if (Current)
    {
      Current = Current->Previous;
      if (Current)
        ResetBOC();
      else
        SetBOC();
    }
    else
    {
      Current = List->Tail;
      if (Current)
        ResetBOC();
      else
        SetBOC();
    };
  };
};
