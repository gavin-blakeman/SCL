//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                bimap.hpp
// LANGUAGE:            C++20
// TARGET OS:           None.
// NAMESPACE:           SCL
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2023-2025 Gavin Blakeman.
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
// OVERVIEW:
//
// HISTORY:             2023-11-17//GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef SCL_BIMAP_HPP
#define SCL_BIMAP_HPP

  // Standard C++ library header files.

#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <type_traits>
#include <unordered_map>

  // SCL Library header files

namespace SCL
{
  template <typename L, typename R> class bimap_iterator;

  /// @brief    The bimap class is a simple implementation of a bi-directional map type class. This relates a left hand side
  ///           (LHS) value to a right hand side (RHS) value. All the values must be unique. All the relationships are fixed and
  ///           1-1. The LHS:RHS pairs can be treated as pairs.
  ///           There container is also small value optimised, with different storage strategies utilised.

  template<typename LHS_T,
           typename RHS_T,
           bool SVO = (sizeof(LHS_T) <= sizeof(std::reference_wrapper<LHS_T>)) &&
                      (sizeof(RHS_T) <= sizeof(std::reference_wrapper<RHS_T>))
           >
  class bimap
  {
  public:
    friend class bimap_iterator<LHS_T, RHS_T>;

    using size_type = std::size_t;
    using value_type = std::pair<LHS_T, RHS_T>;
    using const_iterator = bimap_iterator<LHS_T, RHS_T>;

  private:
    constexpr static bool optimised = SVO;
    typename
    std::conditional<SVO,
                     std::map<LHS_T, RHS_T>,
                     std::map<LHS_T, std::reference_wrapper<value_type>>>::type lhsMap;
    typename
    std::conditional<SVO,
                     std::map<RHS_T, LHS_T>,
                     std::map<RHS_T, std::reference_wrapper<value_type>>>::type rhsMap;


    struct empty_t{};

    [[no_unique_address]] std::conditional_t<SVO, empty_t, std::list<value_type>> valueStorage;


  public:
    bimap() = default;

    //template< class InputIt >
    //bimap(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator() );

    //template< class InputIt >
    //bimap(InputIt first, InputIt last, const Allocator& alloc );

    bimap(const bimap& other) = default;
    bimap(bimap&& other);
    bimap(std::initializer_list<std::pair<LHS_T, RHS_T>> init)
    {
      for (auto &v: init)
      {
        insert(v.first, v.second);
      }
    }

    ~bimap() = default;

    bimap& operator=(bimap const &other) = default;
    bimap& operator=(bimap&& other ) noexcept;
    bimap& operator=(std::initializer_list<value_type> ilist);

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  keyRight: The right key.
    /// @returns    const reference to the left key.
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.
    /// @version    2023-11-17/GGB - Function created.

    LHS_T const &LHS(RHS_T const &keyRight) const
    {
      if constexpr (SVO)
      {
        return rhsMap.at(keyRight);
      }
      else
      {
        return rhsMap.at(keyRight).get().first;
      }
    }

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  keyLeft: The left key of the element to find.
    /// @returns    Reference to the mapped value of the requested element
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.
    /// @version    2023-11-17/GGB - Function created.

    RHS_T const &RHS(LHS_T const &keyLeft) const
    {
      if constexpr (SVO)
      {
        return lhsMap.at(keyLeft);
      }
      else
      {
        return lhsMap.at(keyLeft).get().second;
      }
    }

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  keyLeft: The left key of the element to find.
    /// @returns    Reference to the mapped value of the requested element
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.
    /// @version    2023-11-17/GGB - Function created.

    RHS_T &RHS(LHS_T const &keyLeft)
    {
      if constexpr (SVO)
      {
        return lhsMap.at(keyLeft);
      }
      else
      {
        return lhsMap.at(keyLeft).get().second;
      }
    }

    /// @brief      Checks if the container has no elements, i.e. whether begin() == end().
    /// @returns    true if the container is empty, false otherwise
    /// @throws     None.
    /// @version    2020-09-05/GGB - Function created.

    [[nodiscard]] bool empty() const noexcept
    {
      return lhsMap.empty();
    }

    /// @brief      Returns the number of elements in the container, i.e. std::distance(begin(), end()).
    /// @returns    The number of elements in the container.
    /// @throws     None.
    /// @version    2023-12-04/GGB - Function created.

    [[nodiscard]] size_type size() const noexcept
    {
      return lhsMap.size();
    }

    /// @brief      Erases all elements from the container. After this call, size() returns zero.
    /// @throws     None.
    /// @note       Invalidates any references, pointers, or iterators referring to contained elements. Any past-the-end iterator
    ///             remains valid.
    /// @version    2020-09-05/GGB - Function created.

    void clear() noexcept
    {
      lhsMap.clear();
      rhsMap.clear();
      if constexpr (!SVO)
      {
        valueStorage.clear();
      };
    }

    /*! @brief      Emplaces element into the container, if the container doesn't already contain an element with an equivalent
     *              key.
     *  @param[in]  lk: The LHS key
     *  @param[in]  rk: The RHS key
     *  @complexity O(n)
     *  @throws
     */
    void emplace(LHS_T const &lk, RHS_T const &rk)
    {
      if constexpr (SVO)
      {
        lhsMap.emplace(lk, rk);
        rhsMap.emplace(rk, lk);
      }
      else
      {
        valueStorage.emplace_back(lk, rk);
        lhsMap.emplace(valueStorage.back().first,  std::ref(valueStorage.back()));
        rhsMap.emplace(valueStorage.back().second, std::ref(valueStorage.back()));
      }
    }

    /// @brief      Emplaces element into the container, if the container doesn't already contain an element with an equivalent
    ///             key.
    /// @param[in]  lk: The LHS key
    /// @param[in]  rk: The RHS key
    /// @complexity O(n)
    /// @throws
    /// @version    2024-05-09/GGB - Function created.

    void emplace(LHS_T &&lk, RHS_T &&rk)
    {
      if constexpr (SVO)
      {
        lhsMap.emplace(lk, rk);
        rhsMap.emplace(std::move(rk), std::move(lk));
      }
      else
      {
        valueStorage.emplace_back(std::move(lk), std::move(rk));
        lhsMap.emplace(valueStorage.back().first,  std::ref(valueStorage.back()));
        rhsMap.emplace(valueStorage.back().second, std::ref(valueStorage.back()));
      }
    }

    /// @brief      Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent
    ///             key.
    /// @param[in]  lk: The LHS key
    /// @param[in]  rk: The RHS key
    /// @complexity O(n)
    /// @throws
    /// @version    2020-09-05/GGB - Function created.

    void insert(LHS_T const&lk, RHS_T const &rk)
    {
      if constexpr (SVO)
      {
        lhsMap.emplace(lk, rk);
        rhsMap.emplace(rk, lk);
      }
      else
      {
        valueStorage.emplace_back(lk, rk);
        lhsMap.emplace(lk, std::ref(valueStorage.back()));
        rhsMap.emplace(rk, std::ref(valueStorage.back()));
      };
    }

    /*! @brief      Insert function taking a value_type
     *  @param[in]  v: The value to insert.
     *  @throws
     */

    void insert(value_type const &v)
    {
      insert(v.first, v.second);
    }
//    template<typename _InputIterator,
//            typename = std::_RequireInputIter<_InputIterator>>

#if (__cplusplus >= 202002L)
    template<typename InputIt> requires std::input_or_output_iterator<InputIt>
#else
    template<typename InputIt>
#endif
    void insert(InputIt first, InputIt last)
    {
      std::for_each(first, last, [&](value_type const &v)
      {
        insert(v.first, v.second);
      });
    }

    void insert(std::initializer_list<std::pair<LHS_T, RHS_T>> ilist)
    {
      for (auto &v: ilist)
      {
        insert(v.first, v.second);
      }
    }

    [[nodiscard]] bool contains_LHS(LHS_T const &lk) const noexcept
    {
#if __cplusplus >=202002L
      return lhsMap.contains(lk);
#else
      return lhsMap.find(lk) != lhsMap.end();
#endif
    }

    [[nodiscard]] bool contains_RHS(RHS_T  const &rk) const noexcept
    {
#if __cplusplus >=202002L
      return rhsMap.contains(rk);
#else
      return rhsMap.find(rk) != rhsMap.end();
#endif
    }

    const_iterator begin() { return const_iterator(*this, lhsMap.begin());}
    const_iterator end() { return const_iterator(*this, lhsMap.end()); }

  };  // class bimap

  template<typename L,
           typename R>
  class bimap_iterator
  {
  public:
    using iteratored_type = bimap<L, R>;
    using value_type = typename iteratored_type::value_type;
    using internal_iterator = typename decltype(bimap<L, R>::lhsMap)::const_iterator;
    using const_pointer = value_type const *;
    using const_reference = value_type const &;
    using iterator_category = std::input_iterator_tag;

    bimap_iterator() = delete;
    bimap_iterator(bimap<L, R> const &i, internal_iterator ii) : instance(i), internalIterator(ii) {}
    bimap_iterator(bimap_iterator const &i) = default;

    const_reference operator*() const
    {
      if constexpr (instance.optimised)
      {
        currentValue = *internalIterator;
      }
      else
      {
        currentValue = *internalIterator.get();
      }
      return currentValue;
    }

    const_pointer operator->() const
    {
      if constexpr (instance.optimised)
      {
        currentValue = *internalIterator;
      }
      else
      {
        currentValue = *internalIterator.get();
      }
      return &currentValue;
    }


    bimap_iterator &operator++()
    {
      ++internalIterator;
      return *this;
    }

    bimap_iterator &operator++(int)
    {
      bimap_iterator temp(*this);

      ++internalIterator;
      return temp;
    }

    friend bool operator==(bimap_iterator const &lhs, bimap_iterator const rhs)
    {
      return lhs.internalIterator == rhs.internalIterator;
    }

    friend bool operator!=(bimap_iterator const &lhs, bimap_iterator const rhs)
    {
      return lhs.internalIterator != rhs.internalIterator;
    }


  private:
    bimap<L, R> const &instance;
    internal_iterator internalIterator;
    mutable value_type currentValue;
  };

} // namespace SCL

#endif // SCL_BIMAP_HPP
