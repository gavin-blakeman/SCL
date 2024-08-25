//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                tr.hpp
// LANGUAGE:            C++20
// TARGET OS:           None.
// NAMESPACE:           SCL
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2023-2024 Gavin Blakeman.
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
// HISTORY:             2024-08-23//GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef SCL_TRIMAP_HPP
#define SCL_TRIMAP_HPP

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
  template <typename L, typename C, typename R> class trimap_iterator;

  /// @brief    The trimap class is an implementation of a tri-dimentional map type class. This relates a left hand side
  ///           (LHS) value to a right hand side (RHS) value. All the values must be unique. All the relationships are fixed and
  ///           1-1. The LHS:RHS pairs can be treated as pairs.
  ///           There container is also small value optimised, with different storage strategies utilised.

  template<typename LHS_T,
           typename CHS_T,
           typename RHS_T
           >
  class trimap
  {
  public:
    friend class trimap_iterator<LHS_T, CHS_T, RHS_T>;

    using size_type = std::size_t;
    using value_type = std::tuple<LHS_T, CHS_T, RHS_T>;
    using const_iterator = trimap_iterator<LHS_T, CHS_T, RHS_T>;

  private:
    using value_ref = std::reference_wrapper<value_type>;
    std::map<LHS_T, value_ref> lhsMap;
    std::map<CHS_T, value_ref> chsMap;
    std::map<RHS_T, value_ref> rhsMap;
    std::list<value_type> valueStorage;

  public:
    trimap() = default;

    //template< class InputIt >
    //bimap(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator() );

    //template< class InputIt >
    //bimap(InputIt first, InputIt last, const Allocator& alloc );

    trimap(const trimap& other) = default;
    trimap(trimap&& other);
    trimap(std::initializer_list<std::tuple<LHS_T, CHS_T, RHS_T>> init)
    {
      for (auto &v: init)
      {
        insert(std::get<0>(v), std::get<1>(v), std::get<2>(v));
      }
    }

    ~trimap() = default;

    trimap& operator=(trimap const &other) = default;
    trimap& operator=(trimap&& other ) noexcept;
    trimap& operator=(std::initializer_list<value_type> ilist);

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  keyRight: The right key.
    /// @returns    const reference to the left key.
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.

    LHS_T const &rhs2lhs(RHS_T const &rhsKey) const
    {
      return rhsMap.at(rhsKey).get().first;
    }

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  keyLeft: The left key of the element to find.
    /// @returns    Reference to the mapped value of the requested element
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.

    RHS_T const &lhs2rhs(LHS_T const &lhsKey) const
    {
      return lhsMap.at(lhsKey).get().second;
    }

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  keyLeft: The left key of the element to find.
    /// @returns    Reference to the mapped value of the requested element
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.
    /// @version    2023-11-17/GGB - Function created.

    RHS_T &lhs2rhs(LHS_T const lhsKey)
    {
      return lhsMap.at(lhsKey).get().second;
    }

    /// @brief      Checks if the container has no elements, i.e. whether begin() == end().
    /// @returns    true if the container is empty, false otherwise
    /// @throws     None.
    /// @version    2020-09-05/GGB - Function created.

    [[nodiscard]] bool empty() const noexcept
    {
      return valueStorage.empty();
    }

    /*! @brief      Returns the number of elements in the container, i.e. std::distance(begin(), end()).
     *  @returns    The number of elements in the container.
     *  @throws     noexcept
     */
    [[nodiscard]] size_type size() const noexcept
    {
      return valueStorage.size();
    }

    /*! @brief      Erases all elements from the container. After this call, size() returns zero.
     *  @throws     None.
     *  @note       Invalidates any references, pointers, or iterators referring to contained elements. Any past-the-end iterator
     *              remains valid.
     */
    void clear() noexcept
    {
      lhsMap.clear();
      chsMap.clear();
      rhsMap.clear();
      valueStorage.clear();
    }

    /// @brief      Emplaces element into the container, if the container doesn't already contain an element with an equivalent
    ///             key.
    /// @param[in]  lk: The LHS key
    /// @param[in]  ck: The CHS key.
    /// @param[in]  rk: The RHS key
    /// @complexity O(n)
    /// @throws
    /// @version    2024-05-09/GGB - Function created.

    void emplace(LHS_T &&lk, CHS_T &&ck, RHS_T &&rk)
    {
      valueStorage.emplace_back(std::move(lk), std::move(ck), std::move(rk));
      lhsMap.emplace(std::get<0>(valueStorage.back()),  std::ref(valueStorage.back()));
      chsMap.emplace(std::get<1>(valueStorage.back()), std::ref(valueStorage.back()));
      rhsMap.emplace(std::get<2>(valueStorage.back()), std::ref(valueStorage.back()));
    }

    /// @brief      Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent
    ///             key.
    /// @param[in]  lk: The LHS key
    /// @param[in]  ck: The CHS key.
    /// @param[in]  rk: The RHS key
    /// @complexity O(n)
    /// @throws
    /// @version    2020-09-05/GGB - Function created.

    void insert(LHS_T const&lk, CHS_T const &ck, RHS_T const &rk)
    {
      valueStorage.emplace_back(lk, ck, rk);
      lhsMap.emplace(lk, std::ref(valueStorage.back()));
      chsMap.emplace(ck, std::ref(valueStorage.back()));
      rhsMap.emplace(rk, std::ref(valueStorage.back()));

    }

    /*! @brief      Insert function taking a value_type
     *  @param[in]  v: The value to insert.
     *  @throws
     */

    void insert(value_type const &v)
    {
      insert(std::get<0>(v), std::get<1>(v), std::get<2>(v));
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
        insert(std::get<0>(v),std::get<1>(v), std::get<2>(v));
      });
    }

    void insert(std::initializer_list<std::tuple<LHS_T, CHS_T, RHS_T>> ilist)
    {
      for (auto &v: ilist)
      {
        insert(std::get<0>(v),std::get<1>(v), std::get<2>(v));
      }
    }

    [[nodiscard]] bool contains_LHS(LHS_T const &lk) const noexcept
    {
      return lhsMap.contains(lk);
    }

    [[nodiscard]] bool contains_CHS(CHS_T const &ck) const noexcept
    {
      return chsMap.contains(ck);
    }

    [[nodiscard]] bool contains_RHS(RHS_T  const &rk) const noexcept
    {
      return rhsMap.contains(rk);
    }

    const_iterator begin() { return const_iterator(*this, lhsMap.begin());}
    const_iterator end() { return const_iterator(*this, lhsMap.end()); }

  };  // class bimap

  template<typename L,
           typename C,
           typename R>
  class trimap_iterator
  {
  public:
    using iteratored_type = trimap<L, C, R>;
    using value_type = typename iteratored_type::value_type;
    using internal_iterator = typename decltype(trimap<L, C, R>::lhsMap)::const_iterator;
    using const_pointer = value_type const *;
    using const_reference = value_type const &;
    using iterator_category = std::input_iterator_tag;

    trimap_iterator() = delete;
    trimap_iterator(trimap<L, C, R> const &i, internal_iterator ii) : instance(i), internalIterator(ii) {}
    trimap_iterator(trimap_iterator const &i) = default;

    const_reference operator*() const
    {
      return *internalIterator.get();
    }

    const_pointer operator->() const
    {
      return *internalIterator.get();
    }


    trimap_iterator &operator++()
    {
      ++internalIterator;
      return *this;
    }

    trimap_iterator &operator++(int)
    {
      trimap_iterator temp(*this);

      ++internalIterator;
      return temp;
    }

    friend bool operator==(trimap_iterator const &lhs, trimap_iterator const rhs)
    {
      return lhs.internalIterator == rhs.internalIterator;
    }

    friend bool operator!=(trimap_iterator const &lhs, trimap_iterator const rhs)
    {
      return lhs.internalIterator != rhs.internalIterator;
    }


  private:
    trimap<L, C, R> const &instance;
    internal_iterator internalIterator;
    mutable value_type currentValue;
  };

} // namespace SCL

#endif // SCL_TRIMAP_HPP
