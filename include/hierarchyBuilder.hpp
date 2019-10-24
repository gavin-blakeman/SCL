//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                hierarchyBuilder.hpp
// SUBSYSTEM:           Hierarchy Storage
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
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

#include <map>

namespace SCL
{

  /* I = index type
   * T = dataclass type */

  template<typename I, class T, I nullValue>
  class CHeirarchyLoader
  {
  private:
    struct SNode
    {
      I parentIndex;
      T item;
      std::map<I, SNode> children;
    };

    std::map<I, SNode> root;
    std::multimap<I, SNode> unfound_;
    typedef std::map<I, node>::iterator nodeIterator;
    typedef std::multimap<I, SNode>::iterator unfoundIterator;

  protected:
  public:
    CHeirarchyLoader();
    virtual ~CHierarchyLoader();

    void addItem(I const &itemIndex, I const &parentIndex, T &item)
    {
      if (parentIndex == nullValue)
      {
          // Item has no parent, stick it in the root node.

        root.emplace(itemIndex, parentIndex, item);
      }
      else if (find(root, parentIndex, nodeIterator))
      {
          // Item has a parent and we have found it.
          // Add the item to the child.

        (*nodeIterator).children.emplace(itemIndex, parentIndex, item);

          // Now see if there are any unfound items that can be added to the node that has just been inserted.

        while ((unfoundIterator = unfound_.find(itemIndex)) != unfound_.end())
        {
          (*nodeIterator).children.emplace(*unfoundIterator.second.parentIndex, itemIndex, std::move(*unfoundIterator.second.ite));
          unfound_.erase(unfoundIterator);
        };
      }
      else
      {
          // Have not found a parent yet. Add it to the list of not found items.
          // The parentIndex and item index are reversed to allow the map to be searched easily when new items are added
          // to known nodes. (See above)

        unfound_.emplace(parentIndex, itemIndex, item);
      };
    }

    /// @brief Searches the entire tree for a node with the given index.
    /// @param[in] searchMap: The map to search.
    /// @param[in] nodeIndex: The node index to search for.
    /// @param[out] nodeFound: Iterator pointing at the found node.
    /// @returns true - An node with the passed index was found.
    /// @returns false - No node was found.
    /// @version 2019-10-13/GGB - Function created.

    bool find(std::map<I, SNode> const &searchMap, I nodeIndex, nodeIterator &nodeFound)
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
        nodeIterator mapIterator = searchMap.begin();

        while (mapIterator != searchMap.end() && !returnValue)
        {
          returnValue = find(searchNode.second.children, nodeIndex, nodeFound);
          mapIterator++;
        };
        return returnValue;
      };
    }
  };

} // namespace SCL

#endif // HIERARCHYBUILDER_HPP
