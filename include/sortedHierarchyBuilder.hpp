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
//                      Copyright 2019-2022 Gavin Blakeman.
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

#ifndef SORTEDHIERARCHYBUILDER_H
#define SORTEDHIERARCHYBUILDER_H

  // Standard C++ library header files.

#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>
#include <tuple>
#include <utility>

  // Miscellaneous library header files

//#include "boost/variant.hpp"
#include "fmt/format.h"
#include <GCL>

  // SCL library header files

#include "common.h"
#include "config.h"

namespace SCL
{
  /// @note This implementation fails if more that one child has the same SortOrder value. If the insert() function throws
  /// throw std::runtime_error("Unable to insert item:4");, it is probably related to more than one child with the same SortOrder
  /// and parent.
  /// This can be easily fixed by changing the child_type to a multi-map. This should then work *not tested*, but does lose
  /// determinism of the order.

  /// @class Hierarchy iterator class
  /// @tparam T: Hierarchy class.

  template<typename I,
           typename S,
           typename T,
           typename T_nonconst>
  class sortedHierarchyIterator
  {
  public:
    typedef typename T::elem_type                               elem_type;
    typedef sortedHierarchyIterator<I, S, T, T_nonconst>        self_type;
    typedef T                                                   hierarchyType;
    typedef std::input_iterator_tag                             iterator_category;
    typedef typename hierarchyType::value_type                  value_type;
    typedef typename hierarchyType::size_type                   size_type;
    typedef typename hierarchyType::pointer                     pointer;
    typedef typename hierarchyType::const_pointer               const_pointer;
    typedef typename hierarchyType::reference                   reference;
    typedef typename hierarchyType::const_reference             const_reference;
    typedef typename hierarchyType::difference_type             difference_type;
    typedef typename hierarchyType::node_type                   node_type;
    typedef typename hierarchyType::child_type                  child_type;
    typedef typename hierarchyType::child_type::iterator        child_iterator;

  private:
    hierarchyType *hierarchy_;
    size_type hierarchyLevel_;
    std::stack<child_type *> traverseMapStack_;
    std::stack<child_iterator>  traverseIteratorStack_;
    child_type *currentChildMap_;
    child_iterator currentChildIterator_;

  protected:
  public:
    friend class sortedHierarchyIterator<I, S, T const, T>;

    /// @brief      Constructor for the hierarchyIterator class.
    /// @throws
    /// @version    2019-10-26/GGB - Function created.

    sortedHierarchyIterator() : hierarchy_(nullptr), hierarchyLevel_(0), traverseMapStack_(), traverseIteratorStack_(),
      currentChildMap_(), currentChildIterator_()
    {

    }

    /// @brief      Constructor for the hierarchyIterator class.
    /// @param[in]  hierarchy: The hierarchy to be iterated.
    /// @throws
    /// @version    2019-10-26/GGB - Function created.

    sortedHierarchyIterator(hierarchyType *hierarchy, bool end = false) : hierarchy_(hierarchy), hierarchyLevel_(0), traverseMapStack_(),
      traverseIteratorStack_(), currentChildMap_(&hierarchy->root_), currentChildIterator_()
    {
        // currentChildMap_ is initialised above.

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

    sortedHierarchyIterator(sortedHierarchyIterator<I, S, T, T> const &other)
    {
      throw std::exception("hierarchyIterator Copy constructor not implemented");
    }

    /// @brief      Implements the indirection operator.
    /// @returns    A std::tuple<size_type, I, elem_type>.
    ///                 > The first value is the hierarchy level,
    ///                 > The second value is the index of the item
    ///                 > The third value is the item.
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    std::tuple<size_type, I, elem_type> operator*()
    {
      assert(*this != hierarchy_->end());     // For debugging.

      if (*this != hierarchy_->end())
      {
        return
        {
          hierarchyLevel_,
          std::get<0>(hierarchy_->items_[currentChildIterator_->second.indx]),
          std::get<3>(hierarchy_->items_[currentChildIterator_->second.indx])
        };
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

      if (!currentChildIterator_->second.child || currentChildIterator_->second.child->empty())
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
        currentChildMap_ = (traverseIteratorStack_.top()->second.child).get();
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
  /// @tparam S: The type to use for the sortOrder. Must provide std::less<S>
  /// @tparam T: Type to use as the data to store in the hierarchy.
  /// @tparam nullValue: The type to use as NULL value (top level value)
  /// @tparam sort: If true, the hierarchy will be sorted by index. If false, items will just be added at the end.
  /// @tparam Alloc_: The allocator type to use.

  template<typename I,
           typename S,
           class T,
           bool sort = true,
           class Alloc_ = std::allocator<T>>
  class sortedHierarchy
  {
  public:
    using self_type = sortedHierarchy<I, S, T, sort, Alloc_>;
    typedef T                                                                         elem_type;
    typedef Alloc_                                                                    allocator_type;
    typedef typename Alloc_::value_type                                               value_type;
    using pointer = typename std::allocator_traits<std::allocator<T>>::pointer;
    using const_pointer = typename std::allocator_traits<std::allocator<T>>::const_pointer;
    using reference = elem_type &;
    using const_reference = elem_type const &;
    typedef typename Alloc_::difference_type                                          difference_type;
    typedef typename Alloc_::size_type                                                size_type;
    typedef sortedHierarchyIterator<I, S, self_type, self_type>                       iterator;
    typedef sortedHierarchyIterator<S, const self_type, self_type, const value_type>  const_iterator;
    using itemStorage_t = std::vector<std::tuple<I, I, S, T>>;
    class node_type;
    typedef std::map<S, node_type>                     child_type;
    class node_type
    {
    public:
      size_type indx;
      std::unique_ptr<child_type> child;

    };
    using searchStorage_t = std::map<I, std::size_t>;
    using parent_nodes = std::map<I, node_type *>;
    typedef typename std::multimap<I, size_t>                 unfoundStorage_t;
    typedef typename std::multimap<I, size_t>::iterator       unfoundIterator_t;


    friend class sortedHierarchyIterator<I, S, self_type, self_type>;

  private:
    I rootValue_;                       // The value to used to indicate the root index.
    allocator_type alloc_;
    itemStorage_t items_;               // The vector storing all the information tuple<itemIndex, ParentIndex, SortOrder, T>
    child_type root_;                   // Root of the tree.
    searchStorage_t search_;            // The map allowing fast finding (itemIndex, vectorIndx)
    unfoundStorage_t unfound_;
    parent_nodes parents_;              // ItemIndex, Node
    size_type elementCount_;              // Number of elements stored in the hierarchy. Note: this includes the elements in unfound_
#ifdef SCL_THREAD
    mutable std::mutex classMutex_;
#endif

    /// @brief    Destroys all the allocated elements.
    /// @throws   None.
    /// @version  2022-07-20/GGB - Extended to include sorting parameter.
    /// @version  2019-10-26/GGB - Function created.

    void destroyAllElements()
    {
      root_.clear();
      unfound_.clear();
      search_.clear();
    }

//    /// @brief      Searches the entire tree for a node with the given index.
//    /// @param[in]  searchMap: The map to search.
//    /// @param[in]  nodeIndex: The node index to search for.
//    /// @param[out] nodeFound: Iterator pointing at the found node.
//    /// @returns    true - An node with the passed index was found.
//    /// @returns    false - No node was found.
//    /// @version    2019-10-13/GGB - Function created.

//    bool find(std::map<I, node_type> &searchMap, I nodeIndex, childCollection_iterator &nodeFound)
//    {
//      if ((nodeFound = searchMap.find(nodeIndex)) != searchMap.end())
//      {
//        return true;
//      }
//      else
//      {
//          // The item was not found in the original map. We now need to iterate and search all the nodes.
//          // This is going to be done recursively by calling this function with the successive maps.
//          // The level of recursion should be limited to the depth of the tree. With reasonable depths this should not present
//          // a stack overflow issue.

//        bool returnValue = false;
//        childCollection_iterator mapIterator = searchMap.begin();

//        while (mapIterator != searchMap.end() && !returnValue)
//        {
//          returnValue = find((*mapIterator).second.children, nodeIndex, nodeFound);
//          mapIterator++;
//        };
//        return returnValue;
//      };
//    }

    sortedHierarchy() = delete;
    sortedHierarchy(sortedHierarchy const &other) = delete;
    sortedHierarchy(sortedHierarchy &&) = delete;
    sortedHierarchy &operator=(self_type const &other) = delete;

  protected:
  public:
    /// @brief      Class constructor. Emulates a default constructor for a default constructable object.
    /// @param[in]  rootValue: The index value to use as the root value.
    /// @throws
    /// @version    2020-04-03/GGB - Function created.

    explicit sortedHierarchy(I rootValue = I()) : rootValue_(rootValue), root_(), unfound_(), elementCount_(0), classMutex_() {}

    /// @brief      Function called to set/change the root value.
    /// @param[in]  rootValue: The new root value.
    /// @throws     std::runtime_error if the class already contains any items.
    /// @pre        The class must be empty. empty() == true.
    /// @version    2020-05-04/GGB - Function created.

    void setRootValue(I rootValue)
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif

      if (empty())
      {
        rootValue_ = rootValue;
      }
      else
      {
        throw std::runtime_error("SCL::hierarchy - Instance must have empty() == true before calling setRootValue()");
      };
    }

    /// @brief      Destructor for the hierarchy class.
    /// @throws     None.
    /// @version    2019-10-26/GGB - Function created

    virtual ~sortedHierarchy()
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      destroyAllElements();
    }

    /// @brief      Clears the structure. Removes all elements from the structure.
    /// @throws     None.
    /// @version    2019-10-26/GGB - Function created.

    void clear() noexcept
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      destroyAllElements();
      elementCount_ = 0;
    }

    /// @brief      Insertion function for elements.
    /// @param[in]  itemIndex: The index of the item to insert.
    /// @param[in]  parentIndex: The parent index of the item to insert.
    /// @param[in]  sortOrder: The order to sort within the parentIndex.
    /// @param[in]  item: The actual item to insert.
    /// @details    Inserts the specified item into the hieararchy. If the parentIndex == nullValue, then the item is inserted
    ///             as a top-level item. If the parentIndex != nullValue then the item is inserted as a child of the parent.
    ///             If the parent does not exist, the item will be added to the 'unfound' list of items.
    ///             Each time an item is sucesfully inserted, the 'unfound' list is checked to see if any items in the 'unfound'
    ///             list can now be inserted.
    /// @version    2022-07-20/GGB - Extended to include sorting parameter.
    /// @version    2022-05-25/GGB - Bug 229:Corrected a logic error that would not iterate the unfound nodes correctly.
    /// @version    2019-10-13/GGB - Function created.

    void insert(I itemIndex, I parentIndex, S sortOrder, T item)
    {
      items_.push_back(std::make_tuple(itemIndex, parentIndex, sortOrder, item));  // Store the item in the flat storage.
      size_type indx = items_.size() - 1;
      search_.emplace(itemIndex, indx);                                           // Store the index of the item in the searchStorage.

      if (insertNode(itemIndex, parentIndex, sortOrder, indx, false))
      {
        auto unfoundIterator = unfound_.begin();

        while (unfoundIterator != unfound_.end())
        {
          if (parents_.contains(unfoundIterator->first))
          {
            size_type nindex = unfoundIterator->second;

            if (insertNode(std::get<0>(items_[nindex]),
                           std::get<1>(items_[nindex]),
                           std::get<2>(items_[nindex]),
                           nindex,
                           true))
            {
              unfound_.erase(unfoundIterator);
              unfoundIterator = unfound_.begin();
            }
            else
            {
              ++unfoundIterator;
            }
          }
          else
          {
            ++unfoundIterator;
          }
        };
      };
    }

    /// @brief      Insert a node.
    /// @param[in]  itemIndex: The index of the item to insert.
    /// @param[in]  parentIndex: The parent index of the item to insert.
    /// @param[in]  sortOrder: The order to sort within the parentIndex.
    /// @param[in]  indx: The actual item to insert.
    /// @param[in]  unfound: if false, the item will be inserted into the unfound list.
    /// @details    Inserts the specified item into the hieararchy. If the parentIndex == nullValue, then the item is inserted
    ///             as a top-level item. If the parentIndex != nullValue then the item is inserted as a child of the parent.
    ///             If the parent does not exist, the item will be added to the 'unfound' list of items.
    ///             Each time an item is sucesfully inserted, the 'unfound' list is checked to see if any items in the 'unfound'
    ///             list can now be inserted.
    /// @version    2022-07-20/GGB - Extended to include sorting parameter.
    /// @version    2022-05-25/GGB - Function created.

    bool insertNode(I itemIndex, I parentIndex, S sortOrder, size_type indx, bool unfound)
    {
      bool returnValue = false;

//      DEBUGMESSAGE("\nParent Nodes");
//      for (auto const &p : parents_)
//      {
//        DEBUGMESSAGE(std::to_string(p.first));
//      }

//      DEBUGMESSAGE("ItemIndex: " + std::to_string(itemIndex) + " ParentIndex: " + std::to_string(parentIndex) +
//                   " SortOrder: " + std::to_string(sortOrder) + " unfound: " + (unfound?"True":"False"));

      if (parentIndex == rootValue_)
      {
          // Item is a root node

        auto emplaceValue = root_.emplace(sortOrder, node_type{indx, std::unique_ptr<child_type>()});
        parents_.emplace(itemIndex, &(emplaceValue.first->second));
        if (!emplaceValue.second)
        {
          throw std::runtime_error("Unable to insert item: 1");
        }

        if (unfound_.contains(itemIndex))
        {
          emplaceValue.first->second.child = std::make_unique<child_type>();
          child_type *ptr = emplaceValue.first->second.child.get();

          while (unfound_.contains(itemIndex))
          {
              // The node just inserted has unfound nodes ie children.
              // This is split out for readability.

            auto childNode = unfound_.find(itemIndex);  // iterator

            size_type childIndx = childNode->second;
            S childSort = std::get<2>(items_[childIndx]);

            auto ev = ptr->emplace(childSort, node_type{childIndx, std::unique_ptr<child_type>()});
            if (!ev.second)
            {
              throw std::runtime_error("Unable to insert item: 2");
            }
            parents_.emplace(std::get<0>(items_[childIndx]), &(ev.first->second));
            unfound_.erase(childNode);
          };
        };

        returnValue = true;
      }
      else if (parents_.contains(parentIndex))
      {
        if (!parents_[parentIndex]->child)
        {
          parents_[parentIndex]->child = std::make_unique<child_type>();
        }
        child_type *ptr = parents_[parentIndex]->child.get();

        //auto emplaceValue = ptr->emplace(sortOrder, node_type(indx, std::unique_ptr<child_type>()));
        auto emplaceValue = ptr->emplace(sortOrder, node_type{indx, std::unique_ptr<child_type>()});
        if (!emplaceValue.second)
        {
          DEBUGMESSAGE(fmt::format("Unable to insert item: {:d}. Parent Index: {:d}. Sort Order: {:d}. Probably a duplicate.",
                                   itemIndex, parentIndex, sortOrder));

          throw std::runtime_error("Unable to insert item: " + std::to_string(parentIndex) + ". Probably a duplicate.");
        }
        parents_.emplace(itemIndex, &(emplaceValue.first->second));

          // Now see if there are any unfound items that can be added to the node that has just been inserted.

        if (unfound_.contains(itemIndex))
        {
          if (!emplaceValue.first->second.child)
          {
            emplaceValue.first->second.child = std::make_unique<child_type>();
          };
          child_type *ptr = emplaceValue.first->second.child.get();

          while (unfound_.contains(itemIndex))
          {
              // The node just inserted has unfound nodes ie children.
              // This is split out for readability.

            auto childNode = unfound_.find(itemIndex);  // iterator

            size_type childIndx = childNode->second;
            S childSort = std::get<2>(items_[childIndx]);

//            DEBUGMESSAGE("Child Index: " + std::to_string(childIndx));
//            DEBUGMESSAGE("Child Sort:" + std::to_string(childSort));

            auto ev = ptr->emplace(childSort, node_type{childIndx, std::unique_ptr<child_type>()});
            if (!ev.second)
            {
              DEBUGMESSAGE("ItemIndex: " + std::to_string(itemIndex));
              throw std::runtime_error("Unable to insert item:4");
            }
            parents_.emplace(std::get<0>(items_[childIndx]), &(ev.first->second));

            unfound_.erase(childNode);
          };
        };

        returnValue = true;
      }
      else
      {
          // Have not found a parent yet. Add it to the list of not found items.
          // The parentIndex is stored as the key.

        if (!unfound)
        {
          unfound_.emplace(parentIndex, indx);
        };
      };

      return returnValue;
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

#endif // SORTEDHIERARCHYBUILDER_H
