
// Standard C++ library

#include <functional>
#include <list>
#include <map>
#include <stack>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <vector>

// Miscellaneous libraries

#include <GCL>

#include "include/vectorSorted.hpp"

namespace SCL
{
  /// @brief      Implements a class that accepts parent:child input data and provides various traversal outputs for traversing the
  ///             parent child relationship.
  ///             The class does not copy the original data, but accepts a reference and holds a reference to the original data.
  ///             The data has to have 2 columns. ID, ParentID. These need to be accessible by using get<II>, get<PI>
  ///             The ID value (get<II>()) must be unique. The ParentID can be repeated.
  ///             IE. the class must implement a function template<int N>get(), or similar.
  ///             It is also possible to have sorted and unsorted child orders. If Sorted, then the valueType must implement
  ///             std::less to provide the sort order. IF !Sorted then the sort order may be consistent.
  /// @tparam     I: ID type. Must provide std::less
  /// @tparam     V: The value type. Any type of structure, provided that get<int>(valueType) is implemented. Must provide
  ///                std::less if Sorted is true.
  /// @tparam     II: The get index for the ID field.
  /// @tparam     PI: The get index for the ParentID field.
  /// @param      Sorted: true of the SortOrder must be used.
  /// @tparam     C: Collection type. The type of collection providing the data. Must support cbegin() and cend(). Does not need
  ///                to be an ordered type.
  /// @param      Compare: The comparison function to use.
  ///
  /// @note       If multiple hierarchies are being managed int he same collection, then the get<PI>() may not return a value. In
  ///             these cases, the get<PI> function should throw an exception. The code will catch the exception and not allow it
  ///             to propogate. Additionally the item that threw an exception will not be included in the PID list.
  ///             This functionality is also needed for hierarchies that may have multiple hierarchies.


  template <typename I,
  class V,
  unsigned int II,
  unsigned int PI,
  class C = std::vector<V>,
  bool Sorted = true,
  class Compare = std::less<I>
  >
  class parentChild
  {
  public:
    using valueType = V;
    using idType = I;
    using idRef = I &;
    using input_type = C;
    using entryType = std::tuple<idType, idType, valueType>;
    using vectorType = std::vector<valueType>;
    using listType = std::list<valueType>;
    using inputDataType = C;
    using stackType = std::tuple<idType, std::stack<idType>>;
    using value_reference = std::reference_wrapper<V>;

    using parentType = std::conditional_t<true, idType, idRef>;
    using childType = std::conditional_t<true, idType, idRef>;

    parentChild() = delete;
    parentChild(parentChild const &) = default;
    parentChild(parentChild &&) = default;
    parentChild(input_type &it) : inputData(it) {}  // No content in constructor.

    parentChild &operator=(parentChild const &) = default;
    parentChild &operator=(parentChild &&) = default;

    /// @brief      Returns the parent of the item.
    /// @param[in]  ID: The item to find the parent.
    /// @returns    Vector of the children.
    /// @throws
    /// @version     2024-04-29/GGB - Function created.

    idType parent(idType ID) const
    {
      if (!inputProcessed)
      {
        processInput();
      }

      return idMap.at(ID).get().template get<PI>();
    }

    /// @brief      Returns a vector of the children by ID.
    /// @param[in]  PID: The parent to get the children of.
    /// @returns    Vector of the children.
    /// @throws
    /// @version     2024-04-29/GGB - Function created.

    std::vector<idType> children(idType PID) const
    {
      std::vector<idType> rv;

      if (!inputProcessed)
      {
        processInput();
      }

      rv.reserve(pidMap.at(PID).size());

      for (auto const &child: pidMap.at(PID))
      {
        rv.emplace_back(child.get().template get<II>());
      }

      return rv;
    }

    /// @brief      Invalidates all stored data. Causes recalculation of the hierarchies. Does not delete the input collection.
    /// @throws     None.

    void clear() noexcept
    {
      idMap.clear();
      pidMap.clear();
      inputProcessed = false;
    }

    /// @brief      Pre-order converts the hierarchy to a form P, C1, C11, C12, C2, C12, C22
    /// @param[in]  PID: The parentID for the top of the hierarchy.
    /// @tparam     The template parameter must be an STL like container that maintains order and has a push_back() or
    ///             emplace_back member. The container must have container<valueReference>.
    ///             List, Vector, Deque and forward_list all meet the requirements. The type must be specified as
    ///             container<std::reference_wrapper<const &V>>
    /// @returns    The specified container type with a pre-order traversal.
    /// @note       There does not need to be an ID corresponding to the parentID for the top of the hierarchy. However, there
    ///             must be parentID's that correspond.
    /// @version    2024-02-22/GGB - Function created

    template<typename O>
    //requires ({(O o, valueReference vr) o.push_back(vr)})
    O preOrder(parentType PID) const
    {
      std::stack<idType> stack;
      O returnValue;

      if (!inputProcessed)
      {
        processInput();
      };

      RUNTIME_ASSERT(pidMap.contains(PID), "Unknown or invalid parentID.");

      // Set up the initial stack.

      if (!idMap.contains(PID))
      {
        stackChildren(PID, stack);
      }
      else
      {
        stack.push(PID);
      }

      while (!stack.empty())
      {
        returnValue.template push_back(idMap.at(stack.top()));

        if (hasChildren(stack.top()))
        {
          idType temp = stack.top();
          stack.pop();
          stackChildren(temp, stack);
        }
        else
        {
          stack.pop();
        }
      }
      return returnValue;
    }

    /// @brief      PreOrder converts the hierarchy to a form P, C1, C11, C12, C2, C12, C22. The output is by ID.
    /// @param[in]  PID: The parentID for the top of the hierarchy.
    /// @tparam     The template parameter must be an STL like container that maintains order and has a push_back() or
    ///             emplace_back member.
    ///             List, Vector, Deque and forward_list all meet the requirements. The type must be specified as
    ///             container<idType>
    /// @returns    The specified container type with a pre-order traversal.
    /// @note       There does not need to be an ID corresponding to the parentID for the top of the hierarchy. However, there
    ///             must be parentID's that correspond.
    /// @version    2024-04-29/GGB - Function created

    template<typename O>
    //requires std::is_integer<O::value_type>
    O preOrder_ID(parentType PID) const
    {
      std::stack<idType> stack;
      O returnValue;

      if (!inputProcessed)
      {
        processInput();
      };

      RUNTIME_ASSERT(pidMap.contains(PID), "Unknown or invalid parentID.");

      // Set up the initial stack.

      if (!idMap.contains(PID))
      {
        stackChildren(PID, stack);
      }
      else
      {
        stack.push(PID);
      }

      while (!stack.empty())
      {
        returnValue.template push_back(stack.top());

        if (hasChildren(stack.top()))
        {
          idType temp = stack.top();
          stack.pop();
          stackChildren(temp, stack);
        }
        else
        {
          stack.pop();
        }
      }
      return returnValue;
    }

    /// @brief      PreOrder converts the hierarchy to a form P, C1, C11, C12, C2, C12, C22. The output is byID.
    /// @param[in]  PID: The parentID for the top of the hierarchy.
    /// @tparam     The template parameter must be an STL like container that maintains order and has a push_back() or
    ///             emplace_back member. The container must have container<valueReference>.
    ///             List, Vector, Deque and forward_list all meet the requirements. The type must be specified as
    ///             container<std::pair<ID_t, int_type>>
    /// @returns    The specified container type with a pre-order traversal.
    /// @note       There does not need to be an ID corresponding to the parentID for the top of the hierarchy. However, there
    ///             must be parentID's that correspond.
    /// @version    2024-04-29/GGB - Function created

    template<typename O>
    //requires std::is_integer<O::value_type>
    O preOrder_ID_level(parentType PID) const
    {
      std::stack<idType> stack;
      O returnValue;
      using level_t = decltype(O::value_type::second);
      level_t level = 0;

      if (!inputProcessed)
      {
        processInput();
      };

      RUNTIME_ASSERT(pidMap.contains(PID), "Unknown or invalid parentID.");

      // Set up the initial stack.

      if (!idMap.contains(PID))
      {
        stackChildren(PID, stack);
      }
      else
      {
        stack.push(PID);
      }

      while (!stack.empty())
      {
        returnValue.template push_back(stack.top(), level);

        if (hasChildren(stack.top()))
        {
          idType temp = stack.top();
          stack.pop();
          stackChildren(temp, stack);
          level++;
        }
        else
        {
          stack.pop();
          level--;
        }
      }
      return returnValue;
    }

    /// @brief      Post-order converts the hierarchy to a form C11, C12, C1, C21, C22, C2, P
    /// @param[in]  PID: The parentID for the top of the hierarchy.
    /// @tparam     The template parameter must be an STL like container that maintains order and has a push_back() or
    ///             emplace_back member. The container must have container<valueReference>.
    ///             List, Vector, Deque and forward_list all meet the requirements. The type must be specified as
    ///             container<std::reference_wrapper<const &V>>
    /// @note       There does not need to be an ID corresponding to the parentID for the top of the hierarchy. However, there
    ///             must be parentID's that correspond.
    /// @returns    The specified container type with a pre-order traversal.
    /// @version    2024-02-22/GGB - Function created

    template<typename O>
    O postOrder(parentType PID) const
    {
      std::stack<idType> stack;
      std::stack<parentType> pStack;    // Stack of parents that have childs processed.
      O returnValue;

      /*! Musings: Either hash the entire container to check for changes, or just update every time. For small input collections it
       * is probably quicker to just re-process every time. For larger containers, the better approach would be to hash.
       * The balance is between re-process time and hash time. If re-process time << hash time, then just re-process.
       * Note: Has is the better way to go, as the data within the container needs to be checked for changes.
       *    > If the size of the inputContainer changes, then re-process
       *    > If any of the ID's of the inputContainer change, then re-process.
       *    > If any of the PIDs of the inputContainer change, then re-process.
       * Reading this data is just as long as recreating the hierarchy data.
       * Leave it to the controlling application to invalidate using clear().
       */

      if (!inputProcessed)
      {
        processInput();
      };

      RUNTIME_ASSERT(pidMap.contains(PID), "Unknown or invalid parentID.");

      // Set up the initial stack.

      if (idMap.contains(PID))
      {
        stack.push(PID);
      }
      else
      {
        stackChildren(PID, stack);
      }

      while(!stack.empty())
      {
        if (pStack.empty())
        {
          pStack.push(stack.top());
          stackChildren(stack.top(), stack);
        }
        else if ((pStack.top() != stack.top()) && hasChildren(stack.top()))
        {
          pStack.push(stack.top());
          stackChildren(stack.top(), stack);
        }
        else if (pStack.top() == stack.top())
        {
          while ( !pStack.empty() && !stack.empty() && (pStack.top() == stack.top()))
          {
            returnValue.emplace_back(idMap.at(stack.top()));
            stack.pop();
            pStack.pop();
          };
        }
        else
        {
          returnValue.emplace_back(idMap.at(stack.top()));
          stack.pop();
        }
      }
      return returnValue;
    }

  private:
    using idMap_t = std::map<idType, value_reference>;
    using child_vector = std::conditional_t<Sorted,
        SCL::vector_sorted<value_reference>,
        std::vector<value_reference>>;
    // Select the type of container depending on whether it is sorted or not.
    using pidMap_t = std::map<idType, child_vector>;



    input_type &inputData;            // Reference to the input data. Needs to be non-const.
    mutable idMap_t idMap;            // Storage by ID.
    mutable pidMap_t pidMap;          // Storage by PID.
    mutable bool inputProcessed = false;      // Keeps track of whether the idMap and pidMap are current.

    bool hasChildren(parentType PID) const
    {
      return pidMap.contains(PID) && !pidMap.at(PID).empty();
    }

    /// @brief      Process the input data to create the ID Map (byId) and the parentID map.
    /// @throws
    /// @version    2024-02-22/GGB - Function created.

    void processInput() const
    {
      for (auto &val: inputData)
      {
        idType ID = val.template get<II>();   // Must have an ID type.
        idMap.emplace(ID, std::ref(val));
        try
        {
          parentType PID = val.template get<PI>();    // Does not have to have a parent ID. Only add to the PID list if it has a
          pidMap[PID].push_back(std::ref(val));       // ParentID.
        }
        catch(...) { }
      }
      inputProcessed = true;
    }

    void stackChildren(parentType PID, std::stack<idType> &stack) const
    {
      // Children need to be stacked in reverse order.

      std::for_each(pidMap.at(PID).rbegin(), pidMap.at(PID).rend(), [&stack](value_reference &child)
                    {
        stack.push(child.get().template get<0>());
                    });
    }
  };
}
