//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								AVLTree
// SUBSYSTEM:						AVL Tree Implementatio9n
// LANGUAGE:						C++
// TARGET OS:						none
// LIBRARY DEPENDANCE:	None
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2012-2018 Gavin Blakeman.
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
// OVERVIEW:						This provides a balanced AVL tree implementation for a generic storage library.
//                      The objects to be inserted must be derived from SCL::CSortablePackage
//
// CLASSES INCLUDED:    CAVLTree
//
// CLASS HIERARCHY:     CSortedContainer
//                        - CAVLTree
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-22 GGB - astroManager 0000.00 release.
//                      2012-12-24 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef SCL_AVLTREE_H
#define SCL_AVLTREE_H

#include "Base.h"

namespace SCL
{
  struct SAVLNode;
  typedef SAVLNode *PAVLNode;

  class CAVLTree : public CSortedContainer
  {
  private:
    bool allowDuplicates_;
    PAVLNode root_;

  protected:
  public:
    CAVLTree(bool ownsData = false, bool allowDuplicates = false);
    virtual ~CAVLTree();

    PSortablePackage operator[](size_t);

    virtual bool insert(PSortablePackage);  // Insert an object into the tree.
    virtual bool erase(PSortablePackage);   // Delete an object from the tree.
    virtual void clear();

  };
}

#endif // SCL_AVLTREE_H
