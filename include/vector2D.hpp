//**********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								vector2D
// SUBSYSTEM:						2D vector
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2018, 2020 Gavin Blakeman.
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
// OVERVIEW:            A 2-dimensional STI like vector implementation.
//
// CLASSES INCLUDED:    vector2D - Two dimensional vector
//
// HISTORY:             2018-06-02 GGB - Changed filename and class name from array2DP
//                      2015-09-22 GGB - astroManager 2015.09 release
//                      2015-08-13 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef SCL_VECTOR2D_H
#define SCL_VECTOR2D_H

  // Standard libraries

#include <algorithm>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

  // SCL Library

#include "common.h"
#include "index2D.hpp"

namespace SCL
{
  template<typename T>
  class vector2DIterator
  {
  public:
    typedef vector2DIterator<T>                                 self_type;
    typedef T                                                   VECTYPE;
    typedef typename VECTYPE::value_type                        value_type;
    typedef typename VECTYPE::size_type                         size_type;
    typedef typename VECTYPE::pointer                           pointer;
    typedef typename VECTYPE::const_pointer                     const_pointer;
    typedef typename VECTYPE::reference                         reference;
    typedef typename VECTYPE::const_reference                   const_reference;
    typedef typename VECTYPE::difference_type                   difference_type;
    typedef typename VECTYPE::index_type                        index_type;
    using iterator_category = std::random_access_iterator_tag;
    using elem_type = typename T::value_type;

  private:
    VECTYPE &vector2D_;
    index_type index_;

  protected:
  public:
    //friend class vector2DIterator<const T, T, const elem_type>;

    vector2DIterator(VECTYPE &v2d) : vector2D_(v2d), index_() {}
    vector2DIterator(vector2DIterator<T> const &other) : vector2D_(other.vector2D_), index_(other.index_) {}

    elem_type &operator*() { return (*vector2D_).at(index_); }
    elem_type *operator->() { return *(operator*()); }

    /// @brief prefix increment operator
    /// @returns Iterator after increment
    /// @throws None.
    /// @version 2020-02-29/GGB - Function created.

    self_type &operator++()
    {
      index_.column()++;
      if (index_.column() > vector2D_.vectorSize().column())
      {
        if (index_.row() >= vector2D_.vectorSize().row())
        {
          (*this) = vector2D_.end();
        }
        else
        {
          index_.column() = 0;
          index_.row()++;
        };
      };

      return (*this);
    }

    /// @brief postfix increment operator
    /// @returns Iterator before increment
    /// @throws None.
    /// @version 2020-02-29/GGB - Function created.

    self_type operator++(int)
    {
      self_type tmp(*this);
      ++(*this);
      return tmp;
    }

    /// @brief prefix decrement operator
    /// @returns Iterator after decrement
    /// @throws std::out_of_range - if an attempt is made to decrement before the start of the container.
    /// @version 2020-02-29/GGB - Function created.

    self_type &operator--()
    {
      if (index_.column() == 0)
      {
        if (index_.row() == 0)
        {
          throw std::out_of_range("Attempt to decrement from beginning of container");
        }
        else
        {
          index_.column() = vector2D_.vectorSize_.column();
          index_.row()--;
        };
      }
      else
      {
        index_.column()--;
      };

      return (*this);
    }

    /// @brief postfix decrement operator
    /// @returns Iterator before decrement
    /// @throws std::out_of_range - if an attempt is made to decrement before the start of the container.
    /// @version 2020-02-29/GGB - Function created.

    self_type operator--(int)
    {
      self_type tmp(*this);
      --(*this);

      return tmp;
    }

    self_type operator+(difference_type n) const
    {
      self_type tmp(*this);
      tmp.index_ += n;
      return tmp;
    }

    self_type operator+=(difference_type n)
    {
      index_ += n;
      return *this;
    }

    self_type operator-(difference_type n) const
    {
      self_type tmp(*this);
      tmp.index_ += n;
      return tmp;
    }

    self_type operator-=(difference_type n)
    {
      index_ += n;
      return *this;
    }

    difference_type operator-(const self_type &c) const
    {
      return index_ - c.index_;
    }

    bool operator==(const self_type &other) const
    {
      return index_ == other.index_ && vector2D_ == other.vector2D_;
    }

    /// @brief Not equals operator. Tests if *this is equal to another iterator.
    /// @param[in] other: The instance to test
    /// @returns true if not equal.
    /// @version 2020-02-29/GGB - Function created.

    bool operator!=(const self_type &other) const
    {
      return index_ != other.index_ && vector2D_ == other.vector2D_;
    }

    bool operator>(const self_type &other) const
    {
      return index_ > other.index_;
    }

    bool operator>=(const self_type &other) const
    {
      return index_ >= other.index_;
    }
    bool operator<(const self_type &other) const
    {
      return index_ < other.index_;
    }
    bool operator<=(const self_type &other) const
    {
      return index_ <= other.index_;
    }
  };

//  template <typename vector2DIterator_t>
//  vector2DIterator_t operator+ (const typename vector2DIterator_t::difference_type &a, vector2DIterator_t const &b)
//  {
//    return vector2DIterator_t(a) + b;
//  }

//  template <typename vector2DIterator_t>
//  vector2DIterator_t operator- (const typename vector2DIterator_t::difference_type &a, vector2DIterator_t const &b)
//  {
//    return vector2DIterator_t(a) - b;
//  }

  /// @brief    The vector2D is an STL-like 2D vector implementation.
  /// @details  The class is used for storing two dimensional vectors. The vector size is not specified at creation or compile time.
  ///           The vector behaves like a std::vector, supporting resizing on the fly. This is done by having a look-up-table
  ///           approach and storing the data in a flat data structure. The vector2D specifically breaks the 'data is contiguous'
  ///           guarantee that std::vector provides. As a result, the vector2D does not provide access to the underlying data
  ///           storage.
  ///           The lookup mechanism is done by using a SCL::index2D<size_t> as the index and using an unordered_map to
  ///           provide the lookup. A hash function is provided for the SCL::index2D<> to ensure lookups are correct.
  ///           Empty positions in the flat data structure are also tracked, allowing insertions later on as a way to minimise
  ///           memory usage, while keeping a focus on speed. Likewise, deletions are done by flagging the deleted position as
  ///           available.
  ///           The methods chosen appear to be appropriate for both sparse and dense vectors. A dense vector may be better managed
  ///           by using a fixed array as the lookup table, but resizing and copying as the vector grows could be time and memory
  ///           intensive.
  /// @note     1. Data is not stored in a contiguous fashion.
  /// @note     2. The container supports sparse as well as dense arrays.
  /// @note     3. It is not possible to access the underlying storage directly. THe API must be used to access stored objects.

  template<class T,
           class Alloc_ = std::allocator<T>>
  class vector2D
  {
  public:
    typedef vector2D<T, Alloc_>                                                     self_type;
    typedef Alloc_                                                                  allocator_type;
    typedef typename Alloc_::value_type                                             value_type;
    typedef typename Alloc_::pointer                                                pointer;
    typedef typename Alloc_::const_pointer                                          const_pointer;
    typedef typename Alloc_::reference                                              reference;
    typedef typename Alloc_::const_reference                                        const_reference;
    typedef typename Alloc_::difference_type                                        difference_type;
    typedef typename Alloc_::size_type                                              size_type;
    typedef typename SCL::index2D<size_type>                                        index_type;
    typedef vector2DIterator<self_type>                                             iterator;
    //typedef vector2D<const self_type, self_type, const value_type>                  const_iterator;
    typedef std::reverse_iterator<iterator>                                         reverse_iterator;
    //typedef std::reverse_iterator<const_iterator>                                   const_reverse_iterator;

  private:
    std::unordered_map<index_type, size_type> lookupIndex_;   ///< Lookup index unordered map.
    allocator_type alloc_;                                    ///< Allocator to use.
    std::vector<T> dataArray_;                                ///< Pointer to the data
    SCL::index2D<size_t> vectorSize_;                         ///< The current size of the vector. (Max rows, columns)
    std::vector<size_type> unusedSlots_;                      ///< Unused slots in the dataArray_
    size_type firstSlot_;                                     ///< The first 'unused' slot in the data array.

    /// @brief Destroys all allocated elements.
    /// @throws None.
    /// @version 2020-02-15/GGB - Use a lookup table rather than an array approach.
    /// @version 2018-06-02/GGB - Function changed to templated class vector2D.

    void destroyAllElements()
    {
      dataArray_.clear();
      unusedSlots_.clear();
      lookupIndex_.clear();
      firstSlot_ = 0;
      vectorSize_.row() = vectorSize_.column() = 0;
    }

  protected:
  public:

    /// @brief Default constructor for the class.
    /// @throws None.
    /// @version 2020-02-16/GGB - Function created.

    vector2D() noexcept(noexcept(Alloc_())) :
      lookupIndex_(), dataArray_(), vectorSize_(0, 0), unusedSlots_(),firstSlot_(0)
    {
    }

    explicit vector2D(Alloc_ const &) noexcept;
    explicit vector2D(index_type, T const & = T(), Alloc_ const & = Alloc_());

    /// @brief Constructor for the class.
    /// @param[in] initialSize: The initial size of the vector.
    /// @param[in] allocator: The allocator to use for allocations.
    /// @details The initial numer of rows and columns is used to allocate the initial amount of memory. This does not limit the
    ///          allowed size of the vector.
    /// @throws std::bad_alloc
    /// @version 2020-02-16/GGB - Function changed to STL type constructor and use of sparse array approach.
    /// @version 2018-06-02/GGB - Function changed to templated class vector2D.
    /// @version 2015-07-07/GGB - Function created.

    vector2D(index_type initialSize, Alloc_ const &allocator = Alloc_()) :
      lookupIndex_(), dataArray_(initialSize.row() * initialSize.column()), vectorSize_(initialSize), unusedSlots_(), firstSlot_(0)
    {
    }

    vector2D(vector2D const &);
    vector2D(vector2D &&) noexcept;
    vector2D(vector2D &&, Alloc_ const &alloc);

    /// @brief Class destructor. Ensures all elements are destroyed and the class is ready for destruction.
    /// @throws None.
    /// @version 2020-02-16/GGB - Function created.

    virtual ~vector2D()
    {
      destroyAllElements();
    }

    vector2D &operator=(vector2D const &);
    vector2D &operator=(vector2D &&) noexcept;
    vector2D &operator=(std::initializer_list<T>);

    void assign(index_type, T const &);

    [[nodiscard]] bool empty() const noexcept { return lookupIndex_.empty(); }
    size_type size() const noexcept { return lookupIndex_.size(); }
    size_type max_size() const noexcept { return lookupIndex_.max_size(); }
    void reserve(size_type newCapacity);
    size_type capacity() const noexcept { return dataArray_.capacity(); }
    void shrink_to_fit();

    /// @brief Erases all elements from the container. After this call, size() returns zero.
    /// @note Leaves the capacity of the container unchanged.
    /// @throws None.
    /// @version 2020-02-16/GGB - Function created.

    void clear() noexcept
    {
      destroyAllElements();
    }

    void resize(size_t);
    void resize(size_type, const value_type &);

    /// @brief Returns a reference to the element at the specified index, with bounds checking.
    /// @returns std::optional Reference to the element at the specified index.
    /// @returns If no element exists at the specified index, the optional returns false.
    /// @throws std::out_of_range if index falls outside of the current bounds of the vector.
    /// @throws std::out_of_range if there is no element at the index.
    /// @version 2020-02-16/GGB - Function created.

    reference at(index_type index)
    {
      if ( (index.row() > vectorSize_.row()) ||
           (index.column() > vectorSize_.column()) )
      {
        throw std::out_of_range("index falls outside bounds of vector2D");
      }
      else
      {
        size_type lookupIndex;

        if (lookupIndex = lookupIndex_.at(index))   // Note this will throw an exception std::out_of_range if the container does
        {                                           // not have the specified key
            // lookupIndex is the index into the dataArray_ that we are looking for.

          return dataArray_.at(lookupIndex);
        };
      };
    }

    /// @brief Returns a reference to the element at the specified index, with bounds checking.
    /// @returns std::optional Reference to the element at the specified index.
    /// @returns If no element exists at the specified index, the optional returs false.
    /// @throws std::out_of_range if index falls outside of the current bounds of the vector.
    /// @throws std::out_of_range if there is no element at the index.
    /// @version 2020-02-16/GGB - Function created.

    const_reference at(index_type index) const
    {
      if ( (index.row() > vectorSize_.row()) ||
           (index.column() > vectorSize_.column()) )
      {
        throw std::out_of_range("index falls outside bounds of vector2D");
      }
      else
      {
        size_type lookupIndex;

        if (lookupIndex = lookupIndex_.at(index))   // Note this will throw an exception std::out_of_range if the container does
        {                                           // not have the specified key
            // lookupIndex is the index into the dataArray_ that we are looking for.

          return dataArray_.at(lookupIndex);
        };
      };
    }

    /// @brief Returns a reference to the elemnet at the specified location.

    reference &operator[] (index_type index);
    const_reference &operator[] (index_type) const;

    /// @brief Constructs an object in-place.
    /// @param[in] row: The row to insert th item into the container.
    /// @param[in] column: The column to insert the item into the container.
    /// @param[in] value: The object to insert.
    /// @returns An pair containing an iterator to the inserted object and a bool indicating if the insertion occurred.
    /// @version 2020-02-18/GGB - Function created.

    std::pair<iterator, bool> insert(size_type row, size_type column, T &&value)
    {
      return insert(index_type(row, column), std::move(value));
    }

    /// @brief Constructs an object in-place.
    /// @param[in] index: The index to insert into the container.
    /// @param[in] value: The object to insert.
    /// @returns An pair containing an iterator to the inserted object and a bool indicating if the insertion occurred.
    /// @version 2020-02-18/GGB - Function created.

    std::pair<iterator, bool> insert(index_type index, T &&value)
    {
      size_type insertSlot;
      bool valueInserted = false;
      iterator validIterator(*this);

      if (unusedSlots_.empty())
      {
          // No unused slots, insert at the end of the vector.

        dataArray_.push_back(std::move(value));
        insertSlot = dataArray_.size() - 1;
        lookupIndex_.insert_or_assign(index, insertSlot);
        valueInserted = true;
      }
      else
      {
          // Utilise an unused slot for the insertion.
      };

      return std::make_pair(validIterator, valueInserted);
    }

    /// @brief Emplace implementation taking an index_type as the argument.
    /// @param[in] row: The row to emplace insert the item.
    /// @param[in] column: The row to emplace insert the item.
    /// @param[in] args: The arguments for the constructor.
    /// @throws std::bad_alloc
    /// @version 2020-02-20/GGB - Function created.

    template<class... Args>
    std::pair<iterator, bool> emplace(size_type row, size_type column, Args&&... args)
    {
      return emplace(index_type(row, column), std::forward<Args>(args)...);
    }

    /// @brief Emplace implementation taking an index_type as the argument.
    /// @param[in] index: The index to emplace insert the item.
    /// @param[in] args: The arguments for the constructor.
    /// @throws std::bad_alloc
    /// @version 2020-02-20/GGB - Function created.

    template<class... Args>
    std::pair<iterator, bool> emplace(index_type index, Args&&... args)
    {
      size_type insertSlot;
      bool valueInserted = false;
      iterator validIterator(*this);

      if (unusedSlots_.empty())
      {
          // No unused slots, insert at the end of the vector.

        dataArray_.emplace_back(std::forward<Args>(args)...);
        insertSlot = dataArray_.size() - 1;
        lookupIndex_.insert_or_assign(index, insertSlot);
        valueInserted = true;
      }
      else
      {
          // Utilise an unused slot for the insertion.
      };

      return std::make_pair(validIterator, valueInserted);
    }


  };

}	// namespace SCL


#endif // SCL_VECTOR2D_H
