//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								mapAsVector.hpp
// SUBSYSTEM:						Map implementation using vectors as the underlying storage structure.
// LANGUAGE:						C++20
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2020-2022 Gavin Blakeman.
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
// OVERVIEW:						Implementation of a std::map drop in class that uses vectors for the underlying implementation. This
//                      provides some efficiency gains over the std::map implementation, specically in memory usage. However there
//                      is an insertion and deletion speed penalty that occurs.
//
// CLASSES INCLUDED:		mapAsVector - Implements an STL drop in map function.
//
//
// HISTORY:             2020-09-05 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef MAP_AS_VECTOR_HPP
#define MAP_AS_VECTOR_HPP

  // Standard C++ library header files.

#include <algorithm>
#include <vector>

  // SCL Library header files

#include "vectorSorted.hpp"

namespace SCL
{

  /// @brief    The mapAsVector class is and STL-like implementation of a map type class offering more efficient traversal than
  ///           the std::map implementation. This offers O(N) insertion and deletiontime and O(log N) search time.
  ///           mapAsVector is a sorted associative container that contains key-value pairs with unique keys. Keys are sorted by
  ///           using the comparison function Compare.
  ///           Everywhere the standard library uses the Compare requirements, uniqueness is determined by using the equivalence
  ///           relation. In imprecise terms, two objects a and b are considered equivalent (not unique) if neither compares less
  ///           than the other: !comp(a, b) && !comp(b, a).
  ///           mapAsVector meets the requirements of Container, AllocatorAwareContainer, AssociativeContainer and
  ///           ReversibleContainer.
  /// @details  This is implmented as a std::vector that stores the objects and a vector_sorted that stores the keys. Sorting is
  ///           performed on insertion.
  ///           This will be fairly fast for both insertion and search and can always be extended to use more efficient storage and
  ///           search methods if required.
  ///           The std::vector is used to store the data in the original insertion order, the sorted_vector implements a
  ///           permutation vector that links the Key to the values.
  /// @tparam   reverseLookup: If true, reverse lookup from the value to key is available. This creates additional storage overhead.
  /// @note     1. This is a drop-in for std::map
  /// @note     2. Due to the design of the class guarantees are made that the data is available as sorted data, or in the original
  ///              insertion order. This has advantages if the data set is a hierarchy that needs to be preserved as a hierarchy,
  ///              while allowing O(log n) random access to elements identified by the Key value.
  /// @note     3. This class works well where insertions can be in order or amortised over a large number of searches.

  template<class Key,
           class T,
           class Compare = std::less<Key>,
           class Allocator = std::allocator<T>,
           bool reverseLookup = false>
  class mapAsVector
  {
  public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<Key, T>;
    using value_storage = std::vector<T>;
    using size_type = typename value_storage::size_type;
    using key_storage = vector_sorted<std::pair<Key, size_type>, Compare>;
    using difference_type = typename value_storage::difference_type;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type &;
    using const_reference = value_type const &;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = typename key_storage::iterator;
    using const_iterator = typename key_storage::const_iterator;
    using reverse_iterator = typename key_storage::reverse_iterator;
    using const_reverse_iterator = typename key_storage::const_reverse_iterator;
    using node_type = std::pair<Key, T>;
    using insert_return_type = struct {
                                        iterator position;
                                        bool inserted;
                                        node_type node;
                                      };

  private:
    value_storage valueData;
    key_storage keyData;
    Compare comparisonFunction;

  public:
    mapAsVector() : valueData(), keyData() {}
    explicit mapAsVector(Compare const &comp, Allocator const &alloc = Allocator());

    template< class InputIt >
    mapAsVector(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator() );

    template< class InputIt >
    mapAsVector(InputIt first, InputIt last, const Allocator& alloc );

    mapAsVector(const mapAsVector& other );

    mapAsVector( const mapAsVector& other, const Allocator& alloc );
    mapAsVector( mapAsVector&& other );
    mapAsVector( mapAsVector&& other, const Allocator& alloc );
    mapAsVector( std::initializer_list<value_type> init,const Compare& comp = Compare(), const Allocator& alloc = Allocator() );

    mapAsVector( std::initializer_list<value_type> init, const Allocator& );

    ~mapAsVector();

    mapAsVector& operator=(mapAsVector const &other);
    mapAsVector& operator=(mapAsVector&& other ) noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_move_assignable<Compare>::value);
    mapAsVector& operator=(std::initializer_list<value_type> ilist);

    allocator_type get_allocator() const noexcept;

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  key: The key of the element to find.
    /// @returns    Reference to the mapped value of the requested element
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.
    /// @version    2020-09-05/GGB - Function created.

    T& at(Key const &key)
    {
      iterator i = keyData.find(key);

      return valueData.at(i->second);
    }

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  key: The key of the element to find.
    /// @returns    Reference to the mapped value of the requested element
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.
    /// @version    2020-09-05/GGB - Function created.

    const T& at(Key const &key) const
    {
      iterator i = keyData.find(key);

      return valueData.at(i->second);
    }

    /// @brief      Returns a reference to the value that is mapped to a key equivalent to key, performing an insertion if such key
    ///             does not already exist.
    /// @param[in]  key: The key of the element to find.
    /// @returns    Reference to the mapped value of the new element if no element with key key existed. Otherwise a reference to
    ///             the mapped value of the existing element whose key is equivalent to key.
    /// @throws
    /// @version    2020-09-05/GGB - Function created.

    T& operator[](Key const &key)
    {
    }

    T& operator[]( Key&& key );

    /// @brief      Returns an iterator to the first element of the map. If the map is empty, the returned iterator will be equal to
    ///             end().
    /// @returns    Iterator to the first element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    iterator begin() noexcept
    {
      return keyData.begin();
    }

    /// @brief      Returns an iterator to the first element of the map. If the map is empty, the returned iterator will be equal to
    ///             end().
    /// @returns    Iterator to the first element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    const_iterator begin() const noexcept
    {
      return keyData.begin();
    }

    /// @brief      Returns an iterator to the first element of the map. If the map is empty, the returned iterator will be equal to
    ///             end().
    /// @returns    Iterator to the first element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    const_iterator cbegin() const noexcept
    {
      return keyData.begin();
    }

    /// @brief      Returns an iterator to the element following the last element of the map. This element acts as a placeholder;
    ///             attempting to access it results in undefined behavior.
    /// @returns    Iterator to the element following the last element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    iterator end() noexcept
    {
      return keyData.end();
    }

    /// @brief      Returns an iterator to the element following the last element of the map. This element acts as a placeholder;
    ///             attempting to access it results in undefined behavior.
    /// @returns    Iterator to the element following the last element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    const_iterator end() const noexcept
    {
      return keyData.end();
    }

    /// @brief      Returns an iterator to the element following the last element of the map. This element acts as a placeholder;
    ///             attempting to access it results in undefined behavior.
    /// @returns    Iterator to the element following the last element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    const_iterator cend() const noexcept
    {
      return keyData.end();
    }

    /// @brief      Returns a reverse iterator to the first element of the reversed map. It corresponds to the last element of the
    ///             non-reversed map. If the map is empty, the returned iterator is equal to rend().
    /// @returns    Reverse iterator to the first element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    reverse_iterator rbegin() noexcept
    {
      return keyData.rbegin();
    }

    /// @brief      Returns a reverse iterator to the first element of the reversed map. It corresponds to the last element of the
    ///             non-reversed map. If the map is empty, the returned iterator is equal to rend().
    /// @returns    Reverse iterator to the first element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    const_reverse_iterator rbegin() const noexcept
    {
      return keyData.rbegin();
    }

    /// @brief      Returns a reverse iterator to the first element of the reversed map. It corresponds to the last element of the
    ///             non-reversed map. If the map is empty, the returned iterator is equal to rend().
    /// @returns    Reverse iterator to the first element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    const_reverse_iterator crbegin() const noexcept
    {
      return keyData.rbegin();
    }

    /// @brief      Returns a reverse iterator to the element following the last element of the reversed map. It corresponds to the
    ///             element preceding the first element of the non-reversed map. This element acts as a placeholder, attempting to
    ///             access it results in undefined behavior.
    /// @returns    Reverse iterator to the element following the last element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    reverse_iterator rend() noexcept
    {
      return keyData.rend();
    }

    /// @brief      Returns a reverse iterator to the element following the last element of the reversed map. It corresponds to the
    ///             element preceding the first element of the non-reversed map. This element acts as a placeholder, attempting to
    ///             access it results in undefined behavior.
    /// @returns    Reverse iterator to the element following the last element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    const_reverse_iterator rend() const noexcept
    {
      return keyData.rend();
    }

    /// @brief      Returns a reverse iterator to the element following the last element of the reversed map. It corresponds to the
    ///             element preceding the first element of the non-reversed map. This element acts as a placeholder, attempting to
    ///             access it results in undefined behavior.
    /// @returns    Reverse iterator to the element following the last element.
    /// @throws     None.
    /// @note       As expected this function returns the iterator to the first sorted element.
    /// @version    2020-09-05/GGB - Function created.

    const_reverse_iterator crend() const noexcept
    {
      return keyData.rend();
    }

    /// @brief      Checks if the container has no elements, i.e. whether begin() == end().
    /// @returns    true if the container is empty, false otherwise
    /// @throws     None.
    /// @version    2020-09-05/GGB - Function created.

    [[nodiscard]] bool empty() const noexcept
    {
      return valueData.empty();
    }

    /// @brief      Returns the number of elements in the container, i.e. std::distance(begin(), end()).
    /// @returns    The number of elements in the container.
    /// @throws     None.
    /// @version    2020-09-05/GGB - Function created.

    size_type size() const noexcept
    {
      return valueData.size();
    }

    /// @brief      Returns the maximum number of elements the container is able to hold due to system or library implementation
    ///             limitations, i.e. std::distance(begin(), end()) for the largest container.
    /// @returns    Maximum number of elements.
    /// @throws     None.
    /// @version    2020-09-05/GGB - Function created.

    size_type max_size() const noexcept
    {
      return std::min(valueData.max_size(), keyData.max_size());
    }

    /// @brief      Erases all elements from the container. After this call, size() returns zero.
    /// @throws     None.
    /// @note       Invalidates any references, pointers, or iterators referring to contained elements. Any past-the-end iterator
    ///             remains valid.
    /// @version    2020-09-05/GGB - Function created.

    void clear() noexcept
    {
      valueData.clear();
      keyData.clear();
    }

    /// @brief      Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent
    ///             key.
    /// @param[in]  value: element value to insert.
    /// @returns    Returns a pair consisting of an iterator to the inserted element (or to the element that prevented the
    ///             insertion) and a bool denoting whether the insertion took place.
    /// @complexity O(n)
    /// @throws
    /// @version    2020-09-05/GGB - Function created.

    std::pair<iterator, bool> insert(value_type const &value)
    {

    }

    template< class P >
    std::pair<iterator, bool> insert(P &&value);
    std::pair<iterator, bool> insert(value_type &&value);

    iterator insert( const_iterator hint, const value_type& value );
    template< class P >
    iterator insert( const_iterator hint, P&& value );
    iterator insert( const_iterator hint, value_type&& value );

    template< class InputIt >
    void insert( InputIt first, InputIt last );

    void insert( std::initializer_list<value_type> ilist );

    //insert_return_type insert(node_type&& nh);

    //iterator insert(const_iterator hint, node_type &&nh);

    template <class M>
    std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);

    template <class M>
    std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);

    template <class M>
    iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);

    template <class M>
    iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);

    template< class... Args >
    std::pair<iterator,bool> emplace( Args&&... args );

    template <class... Args>
    iterator emplace_hint( const_iterator hint, Args&&... args );

    template <class... Args>
    std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);

    template <class... Args>
    std::pair<iterator, bool> try_emplace(key_type&& k, Args&&... args);

    template <class... Args>
    iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args);

    template <class... Args>
    iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args);

    iterator erase( const_iterator pos );
    iterator erase( iterator pos );
    iterator erase( const_iterator first, const_iterator last );
    size_type erase( const key_type& key );

    //void swap( mapAsVector& other ) noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable<Compare>::value);

    node_type extract( const_iterator position );
    node_type extract( const key_type& x );

    template<class C2>
    void merge( std::map<Key, T, C2, Allocator>& source );

    template<class C2>
    void merge( std::map<Key, T, C2, Allocator>&& source );

    template<class C2>
    void merge( std::multimap<Key, T, C2, Allocator>& source );

    template<class C2>
    void merge( std::multimap<Key, T, C2, Allocator>&& source );

    size_type count( const Key& key ) const;

    template< class K >
    size_type count( const K& x ) const;

    iterator find( const Key& key );
    const_iterator find( const Key& key ) const;
    template< class K > iterator find( const K& x );
    template< class K > const_iterator find( const K& x ) const;

    bool contains( const Key& key ) const;
    template< class K > bool contains( const K& x ) const;

    std::pair<iterator,iterator> equal_range( const Key& key );
    std::pair<const_iterator,const_iterator> equal_range( const Key& key ) const;
    template< class K >
    std::pair<iterator,iterator> equal_range( const K& x );
    template< class K >
    std::pair<const_iterator,const_iterator> equal_range( const K& x ) const;

    iterator lower_bound( const Key& key );
    const_iterator lower_bound( const Key& key ) const;
    template< class K >
    iterator lower_bound(const K& x);
    template< class K >
    const_iterator lower_bound(const K& x) const;

    iterator upper_bound( const Key& key );
    const_iterator upper_bound( const Key& key ) const;
    template< class K >
    iterator upper_bound( const K& x );
    template< class K >
    const_iterator upper_bound( const K& x ) const;


    key_compare key_comp() const;

    //value_compare value_comp() const;




  };  // class mapAsVector

} // namespace SCL

#endif // MAP_FLAT_HPP
