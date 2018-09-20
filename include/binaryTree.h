//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                BTree
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
// OVERVIEW:            1. Items are inserted into the binary tree in the correct sorted position
//                      2. The binary tree iterator will iterate the tree in the correct order. (Asc and Desc)
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-04-02 GGB - Removed all deprecated functions.
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2010-04-28/GGB - Unscrabble Application Release 1.00
//                      2010-04-22/GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef SCL_BINARYTREE_H
#define SCL_BINARYTREE_H

#include "Base.h"

#include <memory>
#include <iterator>

namespace SCL
{
  template<typename T>
  class CBinaryTree : public CSortedContainer
  {
  protected:
    class CBinaryTreeNode;

  public:
    class CBinaryTreeIterator;

    typename std::iterator_traits<T>::value_type;

    //typedef CBinaryTreeIterator iterator;
    //typedef std::ptrdiff_t difference_type;
    //typedef std::size_t size_type;
    //typedef T value_type;
    //typedef T *pointer;
    //typedef T &reference;


    class CBinaryTreeIterator: public CIterator
    {
    private:
      CBinaryTree &binaryTree;
      CBinaryTreeNode *current;
      CBinaryTreeNode *previous;

    protected:
      virtual void moveFirst();
      virtual void moveLast();
      virtual void moveNext();
      virtual void movePrevious();

      CBinaryTreeIterator(CBinaryTreeIterator const &rhs) : CIterator(rhs), binaryTree(rhs.binaryTree), current(rhs.current), previous(rhs.previous) {}

    public:
      CBinaryTreeIterator(CBinaryTree &bt) : CIterator(), binaryTree(bt), current(nullptr), previous(nullptr) {}
      virtual ~CBinaryTreeIterator();

      bool operator==(T const &rhs) { return current->data == rhs; }
      bool operator!=(T const &rhs) { return !((*this) == rhs); }
      T &operator*() { return current->data; }
      iterator &operator++() { moveNext(); return *this; }
      iterator &operator++(int) { CBinaryTreeIterator clone(*this); moveNext(); return clone;  }
    };

  protected:

    class CBinaryTreeNode
    {
    public:
      CBinaryTreeNode *parent;					// Pointer to the parent node. Necessary for iterating.
      T data;			// The data stored
      CBinaryTreeNode *left;						// The left leaf
      CBinaryTreeNode *right;					// The right leaf

      CBinaryTreeNode(CBinaryTreeNode *newParent, T newData) : parent(newParent), data(newData), left(nullptr), right(nullptr) {}
      CBinaryTreeNode() = delete;
      CBinaryTreeNode(CBinaryTreeNode const &) = delete;
    };

  private:
    CBinaryTreeNode *removeNode(CBinaryTreeNode *);

  protected:
    CBinaryTreeNode *root_;													// Root node of the binary tree.

  public:
    CBinaryTree();
    virtual ~CBinaryTree();

    void insert(T);  // Insert an object into the container.
    bool remove(T);
    void clear();
    iterator begin() { return iterator(*this).moveFirst(); }
    iterator end() { return iterator(*this).moveLast(); }
  };

  //********************************************************************************************************************************
  //
  // CBinaryTree
  //
  //********************************************************************************************************************************

  /// @brief Default constructor for the BTree class.
  /// @throws None.
  /// @version 2010-04-20/GGB - Function created.

  template<typename T>
  CBinaryTree<T>::CBinaryTree() :CSortedContainer(), root_(nullptr)
  {
  }

  /// @brief Destructor for the class.
  /// @details Calls the clear() function to delete all the data.
  /// @version 2010-04-22/GGB - Function created.

  template<typename T>
  CBinaryTree<T>::~CBinaryTree()
  {
    clear();		// Ensure all the data is destroyed correctly.
  }

  /// @brief Deletes the tree structure and if necessary the data in the tree.
  /// @details This is an iterative delete function to ensure correct operation on large unbalanced trees.
  /// @version 2017-06-25/GGB - Converted to templated function.
  /// @version 2010-04-25/GGB - Function created.

  template<typename T>
  void CBinaryTree<T>::clear()
  {
    CBinaryTreeNode *current;
    CBinaryTreeNode *previous;

    if (root_)
    {
      current = root_;

        // Move to bottom left of tree and mangle tree structure as we go to make execution faster.

      while (current)
      {
        if ( (!current->left) && (current->right) )
        {
            // Switch the branches around.

          current->left = current->right;
          current->right = nullptr;
        };
        previous = current;
        current = current->left;
      };

      current = previous;

        // Now at bottom left of modified tree.

      while (current)
      {
        if (!current->left && !current->right)
        {
          previous = current->parent;
          if (previous)
          {
            if (previous->left == current)
            {
              previous->left = nullptr;
            }
            else
            {
              previous->right = nullptr;
            }
          }
          else
          {
            previous = nullptr;
          }

          delete current;
          current = previous;
          decPackageCount();		// Decrease the package count.
        };

        if (current)
        {
          if (current->left)
          {
            current = current->left;		// Should not happen!
          }
          else if (current->right)
          {
            current = current->right;
            while (current)
            {
              if ( (!current->left) && (current->right) )
              {
                  // Switch the branches around.

                current->left = current->right;
                current->right = nullptr;
              };
              previous = current;
              current = current->left;
            };
            current = previous;
          };
        };
      };
    };
    root_ = nullptr;
  }

  /// @brief Inserts the item into the appropriate place in the sorted tree.
  /// @details This uses an iterative rather than a recursive routine. By using the iterative routine, greater security is provided
  /// against stack overflows on large trees that are asymmetrical.
  /// @param[in] toInsert - The object to insert into the binary tree.
  /// @throws std::bad_alloc
  /// @version 2017-06-25/GGB - Converted to templated class
  /// @version 2010-04-27/GGB - 1) Changed from recursive routine to iterative routine.
  ///                           2) Deleted function insert(PCPackage, SBTreeNode *)
  /// @version 2010-04-25/GGB - Function created.

  template<typename T>
  void CBinaryTree<T>::insert(T toInsert)
  {
    CBinaryTreeNode *current;
    CBinaryTreeNode *previous;
    CBinaryTreeNode *insertPoint;

    if (root_)
    {
      current = root_;		// Start at the top of the tree.

      while (current)
      {
        previous = current;
        if ( toInsert < current->data )
        {
          current = current->left;
          if (!current)
          {
            insertPoint = &(current->left);
          };
        }
        else
        {
          current = current->right;
          if (!current)
          {
            insertPoint = &(current->right);
          };
        };
      };

      insertPoint = new CBinaryTreeNode(previous, toInsert);

      incPackageCount();
    }
    else
    {
        // Tree is empty

      root_ = new CBinaryTreeNode(nullptr, toInsert);
      incPackageCount();
    };
  }

  /// @brief Removes an item from the tree.
  /// @param[in] toRemove - The item to remove.
  /// @returns true - Success
  /// @returns false - Failure.
  /// @version 2017-06-25/GGB - Converted to templated class.
  /// @version 2010-04-25/GGB - Function created.

  template<typename T>
  bool CBinaryTree<T>::remove(T toRemove)
  {
    bool returnValue = false;
    CBinaryTreeNode *current, **nodeChange;

    if (root_)
    {
      if (root_->data == toRemove)
      {
          // Data is in the root node. We now need to remove the root node.

        root_ = removeNode(root_);
        returnValue = true;
      }
      else
      {
          // Need to search for the node to remove.

        current = root_;

        while (current && current->data != toRemove)
        {
          if (toRemove < current->data)
          {
            current = current->left;
            nodeChange = &current->left;
          }
          else
          {
            current = current->right;
            nodeChange = &current->right;
          }
        }

        if (current->data = toRemove)
        {
          nodeChange = removeNode(current);
          returnValue = true;
        };
      }
    }

    return returnValue;

  }

  /// @brief Removes the specified node from the tree.
  /// @param[in] toRemove - The node to remove.
  /// @returns The value to replace the parent node pointer with.
  /// @version 2017-06-25/GGB - Function created.

  template<typename T>
  typename CBinaryTree<T>::CBinaryTreeNode *CBinaryTree<T>::removeNode(CBinaryTreeNode *toRemove)
  {
    CBinaryTreeNode *returnValue = nullptr;

    if (!toRemove->left && !toRemove->right)
    {
        // Both nodes are empty. Can remove the toRemove node.

      delete toRemove;

    }
    else if (toRemove->left && !toRemove->right)
    {
        // Single subtree.

      returnValue = toRemove->left;
      delete toRemove;
    }
    else if (!toRemove->left && toRemove->right)
    {
        // Single subtree

      returnValue = toRemove->right;
      delete toRemove;
    }
    else
    {
        // Two subtrees. Need to find minimum value in right subtree.

      CBinaryTreeNode *current = toRemove->right;
      CBinaryTreeNode *previous = nullptr;

      while(current->left)
      {
        previous = current;
        current = current->left;
      };

      toRemove->data = current->data;
      previous->left = nullptr;
      delete current;

      returnValue = toRemove;
    }

    return returnValue;
  }

  //********************************************************************************************************************************
  //
  // CBinaryTreeIterator
  //
  //********************************************************************************************************************************

  /// @brief Moves to the first leaf in the tree. This is the leftmost leaf in the tree.
  /// @details Uses an iterated function rather than a recursive function for more stack security.
  /// @version 2017-06-25/GGB - Converted to a templated function.
  /// @version 2010-04-22/GGB - Function created.

  template<typename T>
  void CBinaryTree<T>::CBinaryTreeIterator::moveFirst()
  {
    if (binaryTree.root_)
    {
      current = binaryTree.root_;
      while (current->Left)
      {
        current = current->left;
      };
      SetBOC();
      ResetEOC();
    }
    else
    {
      current = nullptr;		// Nothing in the tree.
      SetEOC();
      SetBOC();
    };
  }

  /// @brief Moves to the last item in the list. This is the rightmost leaf in the tree.
  /// @details Uses an iterated algorithm rather than a recursive algortithm for more stack security.
  /// @version 2017-06-25/GGB - Updated to use templated function.
  /// @version 2010-04-22/GGB - Function created.

  template<typename T>
  void CBinaryTree<T>::CBinaryTreeIterator::moveLast()
  {
    if (binaryTree.root_)
    {
      current = binaryTree->root_;
      while (current->right)
      {
        current = current->right;
      };

      SetEOC();		// End of container is true.
      ResetBOC();
    }
    else
    {
      current = nullptr;
      SetEOC();
      SetBOC();
    };
  }

  /// @brief Moves to the next item in the binary tree.
  /// @details Iterative function
  ///   @li 0. Left branch must have already been explored.
  ///   @li 1. Right == nullptr then move up one level and return value
  ///   @li Right != NULL then moveFirst(right)
  /// @version 2017-06-25/GGB - Updated to templated function.
  /// @version 2010-04-25/GGB - Function created.

  template<typename T>
  void CBinaryTree<T>::CBinaryTreeIterator::moveNext()
  {
    if (!current)
    {
      moveFirst();		// Nothing currently pointed to.
    }
    else
    {

        // Already somewhere in the tree.

      if ( !current->right )		// Left should have been done already
      {
          // At the bottom of a branch. Must be moving upwards.

        if (current == binaryTree.root_)
        {
          SetEOC();
          //return;
        }
        else
        {
          do
          {
            previous = current;
            current = current->parent;
          }
          while ( (current->right == previous) && (current->parent) );

          if ( (!current->parent) && (current->right == previous) )
          {
            SetEOC();		// Moved up the right branch.
          };
          //return;
        };
      }
      else
      {
          // Not at the bottom of a branch, go to the right

        previous = current;
        current = current->right;		// Move down one node

        if (current->left)					// Does the node now have a left branch
        {
            // Iterate down the left branch, or until we hit the bottom.

          while (current->left)
          {
            previous = current;
            current = current->left;
          };
          //return;										// At the bottom.
        }
        else
        {
          //return;										// Nothing down to the left.
        };
      };
    };
  }

  /// @brief Move to the previous item in the tree.
  /// @version 2017-06-25/GGB - Updated to templated function.
  /// @version 2010-04-26/GGB - Function created.

  template<typename T>
  void CBinaryTree<T>::CBinaryTreeIterator::movePrevious()
  {
    if (!current)
    {
      moveLast();		// Nothing currently pointed to.
    }
    else
    {
        // Already somewhere in the tree.

      if ( !current->left )		// Right should have been done already
      {
          // At the bottom of a branch. Must be moving upwards.

        if (current == binaryTree->root_)
        {
          SetBOC();
          //return;
        }
        else
        {
          do
          {
            previous = current;
            current = current->parent;
          }
          while ( (current->parent) && (current->left == previous) );

          if ( (!current->parent) && (current->left == previous) )
          {
            SetBOC();		// Moved up the right branch.
          };
          //return;
        };
      }
      else
      {
          // Not at the bottom of a branch, go to the left

        previous = current;
        current = current->left;		// Move down one node

        if (current->right)					// Does the node now have a right branch
        {
            // Iterate down the right branch, or until we hit the bottom.

          while (current->right)
          {
            previous = current;
            current = current->right;
          };
          //return;										// At the bottom.
        }
        else
        {
          //return;										// Nothing down to the right.
        };
      };
    };
  }


} // namespace SCL

#endif // SCL_BINARYTREE_H
