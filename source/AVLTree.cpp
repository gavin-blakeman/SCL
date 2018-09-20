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
// CLASSES INCLUDED:    none
//
// CLASS HIERARCHY:     none
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-20 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-12-26 GGB - File created.
//
//*********************************************************************************************************************************

#include "../include/AVLTree.h"

  // Standard libraries

#include <cstdlib>
#include <stack>

  // SCL header files

#include "../include/common.h"

#include <GCL>

namespace SCL
{
  //*******************************************************************************************************************************
  //
  // SAVLNode
  //
  // The node type for the AVL Tree
  //
  //*******************************************************************************************************************************

  struct SAVLNode
  {
    int balance;					      // The balance factor for AVL trees.
    PAVLNode nodes[2];          // Pointer to the child nodes
    PSortablePackage data;      // Pointer to the data.
    long _nodesbelow;           // Number of nodes below this node... Used for indexed access.

    SAVLNode();
    SAVLNode(PSortablePackage);
    void insert(PSortablePackage);
    PAVLNode insertBalance(int);
    PAVLNode removeBalance(int, bool &);
    PAVLNode rotateSingle(int);
    PAVLNode rotateDouble(int);
    void adjustBalance(int, int);
  };

  /// Default constructor for node class.
  //
  // 2012-12-26/GGB - Function created.

  SAVLNode::SAVLNode()
  {
    balance = 0;
    _nodesbelow = 1;
    nodes[0] = nullptr;
    nodes[1] = nullptr;
    data = nullptr;
  }

  /// Constructor taking a pointer to the data.
  //
  // 2012-12-26/GGB - Function created.

  SAVLNode::SAVLNode(PSortablePackage newData)
  {
    balance = 0;
    _nodesbelow = 1;
    nodes[0] = nullptr;
    nodes[1] = nullptr;
    data = newData;
  }

  void SAVLNode::adjustBalance (int dir, int bal)
  {
    PAVLNode n = nodes[dir];
    PAVLNode nn = n->nodes[!dir];

    if ( nn->balance == 0 )
      balance = n->balance = 0;
    else if ( nn->balance == bal )
    {
      balance = -bal;
      n->balance = 0;
    }
    else
    {
        /* nn->balance == -bal */
     balance = 0;
     n->balance = bal;
    };

    nn->balance = 0;
  }

  /// Inserts data into the tree at the appropriate point.
  //
  // 2012-12-26/GGB - Function created.

  void SAVLNode::insert(PSortablePackage toInsert)
  {
    int dir = (*data) < (*toInsert);

    if (nodes[dir])
      nodes[dir]->insert(toInsert);
    else
      nodes[dir] = new SAVLNode(toInsert);

    _nodesbelow += 1;
  }

  PAVLNode SAVLNode::insertBalance(int dir)
  {
    PAVLNode n = nodes[dir];
    PAVLNode returnValue;

    int bal = dir == 0 ? -1 : +1;

    if ( n->balance == bal )
    {
      balance = n->balance = 0;
      returnValue = rotateSingle(!dir);
    }
    else
    { /* n->balance == -bal */
     adjustBalance(dir, bal);
     returnValue = rotateDouble(!dir);
    };

    return returnValue;
  }

  PAVLNode SAVLNode::removeBalance(int dir, bool &done)
  {
    PAVLNode n = nodes[!dir];
    int bal = dir == 0 ? -1 : +1;
    PAVLNode returnValue;

    if ( n->balance == -bal )
    {
      balance = n->balance = 0;
      returnValue = rotateSingle(dir);
    }
    else if ( n->balance == bal )
    {
      adjustBalance(!dir, -bal);
      returnValue = rotateDouble(dir);
    }
    else
    { /* n->balance == 0 */
      balance = -bal;
      n->balance = bal;
      returnValue = rotateSingle(dir);
      done = true;
    };

   return returnValue;
  }

  /// Performs a double rotation of the tree.
  //
  // 2012-12-26/GGB - Function created.

  PAVLNode SAVLNode::rotateDouble(int dir)
  {
    PAVLNode save = nodes[!dir]->nodes[dir];

    nodes[!dir]->nodes[dir] = save->nodes[!dir];
    save->nodes[!dir] = nodes[!dir];
    nodes[!dir] = save;

    save = nodes[!dir];
    nodes[!dir] = save->nodes[dir];
    save->nodes[dir] = this;

    return save;
  };

  /// Performs a single rotation of the tree.
  //
  // 2012-12-26/GGB - Function created.

  PAVLNode SAVLNode::rotateSingle(int dir)
  {
    PAVLNode save = nodes[!dir];

    _nodesbelow -= nodes[!dir]->_nodesbelow;
    if (nodes[dir])
      save->_nodesbelow += nodes[dir]->_nodesbelow;

    nodes[!dir] = save->nodes[dir];
    save->nodes[dir] = this;

    return save;
  }

  //*******************************************************************************************************************************
  //
  // CAVLTree
  //
  // An implementation of a balanced binary tree using the AVL algorithm.
  //
  //
  //*******************************************************************************************************************************

  /// Class constructor
  //
  // 2012-12-26/GGB - 1) Added ownsdata and allow duplicates functionallity.
  // 2010-04-26/GGB - Function created.

  CAVLTree::CAVLTree(bool od, bool ad) : CSortedContainer()
  {
    ownsPackages(od);
    allowDuplicates_ = ad;
  }

  /// Class Destructor... Must ensure that all the memory utilised is recovered.
  //
  // 2012-12-26/GGB - Function created.

  CAVLTree::~CAVLTree()
  {
    clear();
  }

  /// EXCEPTIONS: 0x1000 - AVLTree: Index out of Bounds
  //
  // 2013-06-16/GGB - Changed argument to size_t (64 bit compliance)
  // 2012-12-26/GGB - Function created.

  PSortablePackage CAVLTree::operator[](size_t index)
  {
    PAVLNode current = nullptr;
    PSortablePackage returnValue = nullptr;
    size_t currentIndex;

    if ( index >= size() )
    {
      ERROR("SCL", 0x1000);
    }
    else
    {
      if (size() == 0)
        returnValue = nullptr;
      else if (size() == 1)
        returnValue = root_->data;     // Trivial case index == 0
      else
      {
        current = root_;
        currentIndex = current->_nodesbelow -1;
        if (current->nodes[1])
          currentIndex -= current->nodes[1]->_nodesbelow;

        while (!returnValue)
        {
          if (currentIndex == index)
            returnValue = current->data;
          else if (currentIndex > index)
          {
            current = current->nodes[0];    // Move to left
            currentIndex--;
            if (current->nodes[1])
              currentIndex -= current->nodes[1]->_nodesbelow;
          }
          else
          {
            current = current->nodes[1];
            current++;
            if (current->nodes[0])
              currentIndex += current->nodes[0]->_nodesbelow;
          };
        };
      };
    };
    return returnValue;
  }

  /// Removes all the elements from the tree.
  //
  // 2012-12-26/GGB - Function created.

  void CAVLTree::clear()
  {
    std::stack<PAVLNode> nodeStack;
    PAVLNode current;

    nodeStack.push(nullptr);

    if (root_)
    {
      current = root_;

      while (current)
      {
        if (current->nodes[0])
        {
          nodeStack.push(current);
          current = current->nodes[0];
        }
        else if (current->nodes[1])
        {
          nodeStack.push(current);
          current = current->nodes[1];
        }
        else
        {
          if (ownsPackages() && current->data)
            delete current->data;
          current->data = nullptr;
          if (nodeStack.top())
          {
            if (nodeStack.top()->nodes[0] == current)
              nodeStack.top()->nodes[0] = nullptr;
            else
              nodeStack.top()->nodes[1] = nullptr;
          };
          delete current;
          current = nodeStack.top();
          nodeStack.pop();
        };
      };
      root_ = nullptr;
    };
  }

  // Function to insert an item into the tree.
  // The height must be checked during the insertion so that balancing may be performed.
  //
  // 2010-04-26/GGB - Function created.

  bool CAVLTree::insert(PSortablePackage toInsert)
  {
    PAVLNode current;
    PAVLNode nodeStack[64];
    int dirStack[64];
    int top = 0;

    if (!root_)
      root_ = new SAVLNode(toInsert);
    else
    {
      current = root_;

        /* Search for an empty link, save the path */

      for ( ; ; )
      {
          /* Push direction and node onto stack */

        dirStack[top] = (*(current->data)) < (*toInsert);
        nodeStack[top++] = current;

        if ( current->nodes[dirStack[top - 1]] == nullptr )
         break;

        current = current->nodes[dirStack[top - 1]];
      };

        /* Insert a new node at the bottom of the tree */
      current->nodes[dirStack[top - 1]] = new SAVLNode(toInsert);

      if (!current->nodes[dirStack[top - 1]])
        return false;   // Memory not allocated.

      current->_nodesbelow++;

        /* Walk back up the search path */

      while ( --top >= 0 )
      {
          /* Update balance factors */
        nodeStack[top]->balance += dirStack[top] == 0 ? -1 : +1;
        nodeStack[top]->_nodesbelow++;

          /* Terminate or rebalance as necessary */
        if ( nodeStack[top]->balance == 0 )
          break;
        else if ( abs ( nodeStack[top]->balance ) > 1 )
        {
          nodeStack[top] = nodeStack[top]->insertBalance(dirStack[top]);

            /* Fix the parent */

          if ( top != 0 )
            nodeStack[top - 1]->nodes[dirStack[top - 1]] = nodeStack[top];
         else
           root_ = nodeStack[0];

          break;
        };
      };
    };
    incPackageCount();

    return true;
  }

  /// Function to remove an item from the tree
  //
  // 2012-12-26/GGB - Function created.

  bool CAVLTree::erase(PSortablePackage toRemove)
  {
    PAVLNode current;
    PAVLNode up[64];
    int upd[64], top = 0;
    bool done = false;

    if (root_)
    {
      current = root_;

      for ( ; ; )
      {
          /* Terminate if not found */

        if ( current == nullptr )
          return false;
        else if ( (*(current->data)) == (*toRemove) )
          break;

          /* Push direction and node onto stack */

        upd[top] = current->data < toRemove;
        up[top++] = current;

        current = current->nodes[upd[top - 1]];
      };

        /* Remove the node */

      if ( current->nodes[0] == nullptr || current->nodes[1] == nullptr )
      {
          /* Which child is not null? */

        int dir = current->nodes[0] == nullptr;

          /* Fix parent */

        if ( top != 0 )
          up[top - 1]->nodes[upd[top - 1]] = current->nodes[dir];
        else
          root_ = current->nodes[dir];

        if (ownsPackages())
          delete current->data;

        delete current;
      }
      else
      {
          /* Find the inorder successor */

        PAVLNode heir = current->nodes[1];

          /* Save the path */

        upd[top] = 1;
        up[top++] = current;

        while ( heir->nodes[0] != nullptr)
        {
          upd[top] = 0;
          up[top++] = heir;
          heir = heir->nodes[0];
        };

          /* Swap data */
        current->data = heir->data;

          /* Unlink successor and fix parent */

        up[top - 1]->nodes[up[top - 1] == current] = heir->nodes[1];

        delete heir;
      };

        /* Walk back up the search path */
      while ( --top >= 0 && !done )
      {
          /* Update balance factors */

        up[top]->balance += upd[top] != 0 ? -1 : +1;

          /* Terminate or rebalance as necessary */

        if ( abs ( up[top]->balance ) == 1 )
          break;
        else if ( abs ( up[top]->balance ) > 1 )
        {
           up[top] = up[top]->removeBalance(upd[top], done);

            /* Fix parent */

           if ( top != 0 )
            up[top - 1]->nodes[upd[top - 1]] = up[top];
          else
            root_ = up[0];
        };
      };
    };

    incPackageCount();
    return true;
  }

}  // namespace SCL
