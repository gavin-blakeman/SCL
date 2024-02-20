
// Standard C++ library

#include <list>
#include <map>
#include <stack>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "include/vectorSorted.hpp"

namespace SCL
{  
  // Implements a class that accepts parent:child input data and provides various traversal outputs for traversing the parent 
  // child relationship.
  // The class does not copy the original data, but accepts a reference and holds a reference to the original data.
  // The data has to have 3 columns. ID, ParentID, SortOrder. These need to be accessible by using std::get<0>, std::get<1>
  // and std::get<2>. 
  
  /// @tparam   I - ID type. Must be sortable and provide std::less
  /// @tparam   V - The value type. Any type of structure, provided that std::get<>(valueType) is implemented.
  /// @tparam   C - Collection type. The type of collection providing the data. Must support cbegin() and cend(). Does not need 
  ///           to be an ordered type.

  template <typename I,                      
            typename V,
            int II,
            int PI,
            class Compare = std::less<V>,
            class C = std::vector<V>>
  class parentChild
  {
  public:
    using valueType = V;    
    using idType = I;
    using idRef = I &;
    using inputType = C;
    using entryType = std::tuple<idType, idType, valueType>;
    using vectorType = std::vector<valueType>;
    using listType = std::list<valueType>;
    using inputDataType = C;    
    using stackType = std::tuple<idType, std::stack<idType>>;
    using valueReference = std::reference_wrapper<V const>;
    
    using parentType = std::conditional<true, idType, idRef>::type;
    using childType = std::conditional<true, idType, idRef>::type;
    
    parentChild() = delete;
    parentChild(parentChild const &);
    parentChild(parentChild &&);
    
    parentChild(inputType const &);
    
    parentChild &operator=(parentChild const &);
    parentChild &operator=(parentChild &&);
    
    /// @brief      Invalidates all stored data. Causes recalculation of the hierarchies.
    /// @throws     None.
        
    void clear() noexcept
    {
      idMap.clear();
      pidMap.clear();
      inputProcessed = false;
      preOrdered.reset();
      postOrdered.reset();
    }
    
    
    /// @brief      Pre-order converts the hierarchy to a form P, C1, C11, C2, C12 
    /// @param[in]  PID: The parentID for the top of the hierarchy.
    /// @tparam     The template parameter must be an STL like container that maintains order and has a push_back() or 
    ///             emplace_back member.
    ///             List, Vector, Deque and forward_list all meet the requirements. The type must be specified as 
    ///             container<std::reference_wrapper<const &V>>
    /// @returns    The specified container type with a pre-order traversal.
    
    template<typename O>
    //requires ({(O o, valueReference vr) o.push_back(vr)})
    O preOrder(parentType PID) 
    {      
      std::stack<idType> stack;
      O returnValue;
      
      if (!inputProcessed)
      {
        processInput();
      };
      
      // Set up the initial stack.
      
      stack.push(PID);
      
      while (!stack.empty())
      {
        returnValue.push_back(std::get<0>(idMap.at(stack.top())));
                
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
    
    /// @brief      Post-order converts the hierarchy to the form, C11, C1, C22, C2, P. 
    
    template<typename O>
    O postOrder(parentType PID)
    {
      std::stack<idType> stack;
      std::stack<parentType> pStack;    // Stack of parents that have childs processed.
      O returnValue;
      
      if (!inputProcessed)
      {
        processInput();
      };
      
      // Set up the initial stack.
      
      stack.push(PID);
      
      while(!stack.empty())
      {
        if ((pStack.top() != stack.top()) && hasChildren(stack.top()))
        {
          pStack.push(stack.top());
          stackChildren(stack.top());
        }
        else
        {
          returnValue.emplace_back(std::get<0>(idMap.at(stack.top())));
          stack.pop();          
        }
      } 
    }
    
  private:  
    using idMap_t = std::map<idType, valueReference>;
    using pidMap_t = std::map<idType, SCL::vector_sorted<valueReference>>;
    
    inputType &inputData;
    idMap_t idMap;    // Main storage. All others reference the data.
    pidMap_t pidMap;
    bool inputProcessed = false;
    std::optional<std::list<valueReference>> preOrdered;
    std::optional<std::list<valueReference>> postOrdered;
    
    bool hasChildren(parentType PID)
    {
      return !pidMap.at(PID).empty();
    }
    

    void processInput()
    {
//      for (auto const &val: inputData)
//      {
//        idType ID = std::get<0>(val);
//        parentType PID = std::get<1>(val);
//
//        idMap.emplace(ID, std::cref(val));
//        pidMap.emplace(std::piecewise_construct,
//                       std::forward_as_tuple(PID),
//                       std::forward_as_tuple(order, std::cref(val)));
//
//      }
      inputProcessed = true;
    }
    
    void stackChildren(parentType PID, std::stack<idType> &stack)
    {
      // Children need to be stacked in reverse order.

      std::for_each(pidMap.at(PID).rbegin(), pidMap.at(PID).rend(), [&stack](idType id)
      {
        stack.push(id);
      });
    }
  };
}
