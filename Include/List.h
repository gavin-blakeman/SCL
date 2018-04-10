//*****************************************************************************
//
// PROJECT:			Storage Class Library (SCL)
// FILE:				List.h
// SUBSYSTEM:		Linked List Implementation
// TARGET OS:		All
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
// OVERVIEW:            1) List always inserts items at the end of the list.
//                      2) Items can be deleted randomly in the list.
//                      3) The list can be traversed in either direction.
//                      4) Implemented using a doubly-linked list.
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2010-04-28 GGB - Unscrabble Application Release 1.00
//
//*****************************************************************************

#ifndef SCL_LIST_H
#define SCL_LIST_H

#include "Base.h"

namespace SCL
{
  struct SListNode;
  class CListIterator;

  class CList : public CContainer
  {
  private:
    SListNode *Head;
    SListNode *Tail;

  protected:
  public:
    CList(void);
    virtual ~CList(void);

    virtual bool Insert(CPackage *);  // Insert an object into the list
    virtual bool Remove(CPackage *);  // remove an object from the list
    virtual void Empty(void);					// Empty the list.

    friend class CListIterator;
  };

  class CListIterator : public CIterator
  {
  private:
    CList *List;
    SListNode *Current;

  protected:
  public:
    CListIterator(CList *);
    virtual ~CListIterator(void);

    virtual void MoveFirst(void);
    virtual void MoveLast(void);
    virtual void MoveNext(void);
    virtual void MovePrevious(void);

    virtual CPackage *GetFirst(void);
    virtual CPackage *GetLast(void);
    virtual CPackage *GetNext(void);
    virtual CPackage *GetPrevious(void);
    virtual CPackage *GetCurrent(void);

  };
}

#endif // SCL_LIST_H
