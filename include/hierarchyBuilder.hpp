//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                hierarchyBuilder.hpp
// SUBSYSTEM:           Hierarchy Storage
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// NAMESPACE:           SCL
// LICENSE:             GPLv2
//
//                      Copyright 2019 Gavin Blakeman.
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
// OVERVIEW:            A class to allow loading and sorting of hierarchical systems such as multi-level menus. Load the data into
//                      the class in a random sequence and then read out in a guaranteed structured sequence.
//
// HISTORY:             2019-10-13 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef HIERARCHYBUILDER_HPP
#define HIERARCHYBUILDER_HPP

  // Standard C++ library header files.

#include <cassert>
#include <map>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <iostream>

  // SCL library header files

#include "common.h"
#include "config.h"

namespace SCL
{
  /// @brief Node storage class. Stores the node information.
  /// @tparam I: The type used for the index.
  /// @tparam T: The type used for the data.

  template<typename I,
           class T>
  struct SNode                          // Struct is used for storing the data in the unfound list.
  {
    I parentIndex;                      // Parent index. What index owns this particular item
    T item;                             // The actual item.
    std::map<I, SNode> children;        // Children of (*this) node.

    SNode(I pi, T i) : parentIndex(pi), item(i), children() {}
    virtual ~SNode() { children.clear(); }
  };

  /// @class Hierarchy iterator class
  /// @tparam T: Hierarchy class.

  template<typename I,
           typename T,
           typename T_nonconst>
  class hierarchyIterator
  {
  public:
    typedef typename T::elem_type                               elem_type;
    typedef hierarchyIterator<I, T, T_nonconst>                 self_type;
    typedef T                                                   hierarchyType;
    typedef std::input_iterator_tag                             iterator_category;
    typedef typename hierarchyType::value_type                  value_type;
    typedef typename hierarchyType::size_type                   size_type;
    typedef typename hierarchyType::pointer                     pointer;
    typedef typename hierarchyType::const_pointer               const_pointer;
    typedef typename hierarchyType::reference                   reference;
    typedef typename hierarchyType::const_reference             const_reference;
    typedef typename hierarchyType::difference_type             difference_type;


  private:
    typedef SNode<I, elem_type>                                 node_type;
    typedef typename std::map<I, node_type>                     childCollection_type;
    typedef typename childCollection_type::iterator             childCollection_iterator;

    hierarchyType *hierarchy_;
    size_type hierarchyLevel_;
    std::stack<childCollection_type *> traverseMapStack_;
    std::stack<childCollection_iterator>  traverseIteratorStack_;
    childCollection_type *currentChildMap_;
    childCollection_iterator currentChildIterator_;

  protected:
  public:
    friend class hierarchyIterator<I, T const, T>;

    /// @brief Constructor fot the hierarchyIterator class.
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    hierarchyIterator() : hierarchy_(nullptr), hierarchyLevel_(0), traverseMapStack_(),
      traverseIteratorStack_(), currentChildMap_(), currentChildIterator_()
    {

    }

    /// @brief Constructor for the hierarchyIterator class.
    /// @param[in] heirarchy: The hierarchy to be iterated.
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    hierarchyIterator(hierarchyType *hierarchy, bool end = false) : hierarchy_(hierarchy), hierarchyLevel_(0), traverseMapStack_(),
      traverseIteratorStack_(), currentChildMap_(&hierarchy->root_), currentChildIterator_()
    {
      if (end)
      {
        currentChildIterator_ = currentChildMap_->end();
      }
      else
      {
        currentChildIterator_ = currentChildMap_->begin();
      }
    }

    /// @brief Copy constructor. Needed for pushing/popping from the stack.

    hierarchyIterator(hierarchyIterator<I, T, T> const &other)
    {
      throw std::exception("hierarchyIterator Copy constructor not implemented");
    }

    /// @brief Implements the indirection operator.
    /// @returns A std::tuple<size_type, I, elem_type>. The first value is the hierarchy level,
    ///                                                 The second value is the index of the item
    ///                                                 the third value is the item.
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    std::tuple<size_type, I, elem_type> operator*()
    {
      assert(*this != hierarchy_->end());     // For debugging.

      if (*this != hierarchy_->end())
      {
        return { hierarchyLevel_, currentChildIterator_->first, currentChildIterator_->second.item };
      }
      else
      {
        throw std::out_of_range("Dereferencing *end()");
      };
    }

    /// @brief Implements the pointer to operator
    /// @returns A std::pair<size_type, elem_type>. The first value is the hierarchy level, the second value if the item value.
    /// @version 2019-10-26/GGB - Function created.

    std::pair<size_type, elem_type> operator->()
    {
      throw std::exception("hierarchyBuilder operator-> not implemented.");
    }

    /// @brief Implements the ++operator for the class.
    /// @returns (*this)
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    self_type &operator++()
    {
      bool exitLoop;

        // Moving forward through the hierarchy is done by stepping down then forward.

      if ((*currentChildIterator_).second.children.empty())
      {
          // Step forward one.

        currentChildIterator_++;
        if (currentChildIterator_ == currentChildMap_->end())
        {
            // Step up one level if possible, otherwise we have reached end().

          exitLoop = false;
          while ( (hierarchyLevel_ != 0) && !exitLoop)
          {
            currentChildMap_ = traverseMapStack_.top();
            traverseMapStack_.pop();
            currentChildIterator_ = traverseIteratorStack_.top();
            traverseIteratorStack_.pop();
            hierarchyLevel_--;
            currentChildIterator_++;
            if (currentChildIterator_ != currentChildMap_->end())
            {
              exitLoop = true;
            };
          };
        };
      }
      else
      {
          // Step down

        traverseMapStack_.push(currentChildMap_);
        traverseIteratorStack_.push(currentChildIterator_);
        currentChildMap_ = &(traverseIteratorStack_.top()->second.children);
        currentChildIterator_ = currentChildMap_->begin();
        hierarchyLevel_++;
      };

      return *this;
    }

    self_type operator++(int);
    self_type operator+(difference_type);
    self_type operator+=(difference_type);

    /// @brief Implements the == operator for the iterator class.
    /// @param[in] other: The iterator to test against.
    /// @returns true - The iterators are the same.
    /// @returns false - The iterators are not the same.

    bool operator==(const self_type &other) const
    {
      bool returnValue = false;

      if (hierarchy_ == other.hierarchy_)
      {
        if (hierarchyLevel_ == other.hierarchyLevel_)
        {
          returnValue = (currentChildIterator_ == other.currentChildIterator_);
        };
      };

      return returnValue;
    }

    /// @brief Implements the != operator for the iterator class.
    /// @param[in] other: The iterator to test against.
    /// @returns true - The iterators are the same.
    /// @returns false - The iterators are not the same.

    bool operator!=(const self_type &other) const
    {
      return !(*this == other);
    }

    bool operator>(const self_type &other) const;
    bool operator>=(const self_type &other) const;
    bool operator<(const self_type &other) const;
    bool operator<=(const self_type &other) const;
  };    // class hierarchyIterator

  /// @class Template class for creating hierarchies of values.
  /// @tparam I: Type to use for the index. Must at least provide '=='.
  /// @tparam T: Type to use as the data to store in the hierarchy.
  /// @tparam nullValue: The type to use as NULL value (top level value)
  /// @tparam sort: If true, the hierarchy will be sorted by index. If false, items will just be added at the end.

  template<typename I,
           class T,
           I nullValue,
           bool sort = true,
           class Alloc_ = std::allocator<T>>
  class hierarchy
  {
  public:
    typedef hierarchy<I, T, nullValue, sort, Alloc_>                                self_type;
    typedef T                                                                       elem_type;
    typedef Alloc_                                                                  allocator_type;
    typedef typename Alloc_::value_type                                             value_type;
    typedef typename Alloc_::pointer                                                pointer;
    typedef typename Alloc_::const_pointer                                          const_pointer;
    typedef typename Alloc_::reference                                              reference;
    typedef typename Alloc_::const_reference                                        const_reference;
    typedef typename Alloc_::difference_type                                        difference_type;
    typedef typename Alloc_::size_type                                              size_type;
    typedef hierarchyIterator<I, self_type, self_type>                              iterator;
    typedef hierarchyIterator<const self_type, self_type, const value_type>         const_iterator;

    friend class hierarchyIterator<I, self_type, self_type>;

  private:
    typedef SNode<I, value_type>                                node_type;
    typedef typename std::map<I, node_type>                     childCollection_type;
    typedef typename childCollection_type::iterator             childCollection_iterator;
    typedef typename std::multimap<I, node_type>                unfoundStorage_t;
    typedef typename std::multimap<I, node_type>::iterator      unfoundIterator_t;

    allocator_type alloc_;
    childCollection_type root_;                 // The root node.
    unfoundStorage_t unfound_;
    size_type elementCount_;              // Number of elements stored in the hierarchy. Note: this includes the elements in unfound_
#ifdef SCL_THREAD
    mutable std::mutex classMutex_;
#endif

    /// @brief Destroys all the allocated elements.
    /// @throws None.
    /// @version 2019-10-26/GGB - Function created.

    void destroyAllElements()
    {
      root_.clear();
      unfound_.clear();
    }


    /// @brief Searches the entire tree for a node with the given index.
    /// @param[in] searchMap: The map to search.
    /// @param[in] nodeIndex: The node index to search for.
    /// @param[out] nodeFound: Iterator pointing at the found node.
    /// @returns true - An node with the passed index was found.
    /// @returns false - No node was found.
    /// @version 2019-10-13/GGB - Function created.

    bool find(std::map<I, node_type> &searchMap, I nodeIndex, childCollection_iterator &nodeFound)
    {
      if ((nodeFound = searchMap.find(nodeIndex)) != searchMap.end())
      {
        return true;
      }
      else
      {
          // The item was not found in the original map. We now need to iterate and search all the nodes.
          // This is going to be done recursively by calling this function with the successive maps.
          // The level of recursion should be limited to the depth of the tree. With reasonable depths this should not present
          // a stack overflow issue.

        bool returnValue = false;
        childCollection_iterator mapIterator = searchMap.begin();

        while (mapIterator != searchMap.end() && !returnValue)
        {
          returnValue = find((*mapIterator).second.children, nodeIndex, nodeFound);
          mapIterator++;
        };
        return returnValue;
      };
    }

  protected:
  public:
    explicit hierarchy() : root_(), unfound_(), elementCount_(0), classMutex_() {}
    hierarchy(hierarchy const &other) = delete;
    hierarchy &operator=(self_type const &other) = delete;

    /// @brief Destructor for the hierarchy class.
    /// @throws None.
    /// @version 2019-10-26/GGB - Function created

    virtual ~hierarchy()
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      destroyAllElements();
    }

    /// @brief Clears the structure. Removes all elements from the structure.
    /// @throws None.
    /// @version 2019-10-26/GGB - Function created.

    void clear() noexcept
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      destroyAllElements();
      elementCount_ = 0;
    }

    /// @brief Insertion function for elements.
    /// @param[in] itemIndex: The index of the item to insert.
    /// @param[in] parentIndex: The parent index of the item to insert.
    /// @param[in] item: The actual item to insert.
    /// @details Inserts the specified item into the hieararchy. If the parentIndex == nullValue, then the item is inserted
    ///          as a top-level item. If the parentIndex != nullValue then the item is inserted as a child of the parent.
    ///          If the parent does not exist, the item will be added to the 'unfound' list of items.
    ///          Each time an item is sucesfully inserted, the 'unfound' list is checked to see if any items in the 'unfound' list
    ///          can now be inserted.
    /// @version 2019-10-13/GGB - Function created.

    void insert(I itemIndex, I parentIndex, T item)
    {
      childCollection_iterator nodeFound;
      unfoundIterator_t unfoundNode;

      if (parentIndex == nullValue)
      {
          // Item has no parent, stick it in the root node.

        root_.emplace(itemIndex, SNode{parentIndex, item});
      }
      else if (find(root_, parentIndex, nodeFound))
      {
          // Item has a parent and we have found it.
          // Add the item to the child.

        auto emplaceValue = (*nodeFound).second.children.emplace(itemIndex, SNode{parentIndex, item});

          // Now see if there are any unfound items that can be added to the node that has just been inserted.

        while ((unfoundNode = unfound_.find(itemIndex)) != unfound_.end())
        {
        (*emplaceValue.first).second.children.emplace((*unfoundNode).second.parentIndex,
                                                      SNode{itemIndex, std::move((*unfoundNode).second.item)});
          unfound_.erase(unfoundNode);
        };
      }
      else
      {
          // Have not found a parent yet. Add it to the list of not found items.
          // The parentIndex and item index are reversed to allow the map to be searched easily when new items are added
          // to known nodes. (See above)

        unfound_.emplace(parentIndex, SNode{itemIndex, item});
      };
    }

    /// @brief Returns a copy of the allocator object associated with the hierarchy.
    /// @returns A copy of the allocator object.
    /// @throws None.
    /// @version 2019-10-26/GGB - Function created.

    allocator_type get_allocator() const noexcept { return alloc_; }

    /// @brief Returns an iterator to the first item in the hiearachy.
    /// @return An interator to the first item in the hierarchy.
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    iterator begin()
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      return iterator(this);
    }

    /// @brief Returns an iterator one past the last element.
    /// @returns An iterator pointing one past the last element.
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    iterator end()
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      return iterator(this, true);
    }

    /// @brief Returns a const iterator to the first item in the hiearachy.
    /// @return An const interator to the first item in the hierarchy.
    /// @version 2019-10-26/GGB - Function created.

    const_iterator begin() const
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      return const_iterator(this);
    }

    /// @brief Returns a const iterator one past the last element.
    /// @returns An const iterator pointing one past the last element.
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    const iterator end() const
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      return iterator(this, true);
    }

    /// @brief Determines if the hierarchy is empty.
    /// @returns true - hierarchy is empty.
    /// @returns false - hierarchy is not empty.
    /// @throws None.
    /// @version 2019-10-26/GGB - Function created.

    bool empty() const noexcept
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      return (elementCount_ == 0);
    }

    /// @brief Returns the number of elements in the container.
    /// @returns The number of elements in the container.
    /// @version 2019-10-26/GGB - Function created.

    size_type size() const noexcept
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      return elementCount_;
    }

    /// @brief Returns the number of items in the unfound list.
    /// @returns The number of items in the unfound list.
    /// @throws None.
    /// @version 2019-10-26/GGB - Function created.

    size_type unfoundCount() const noexcept
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif

      return unfound_.size();
    }

  };    // class hierarchy


} // namespace SCL

#endif // HIERARCHYBUILDER_HPP
