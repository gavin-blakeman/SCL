//**********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								vector2Dva
// SUBSYSTEM:						2D vector
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	SCL
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    vector2Dva - Two dimensional vector
//
// CLASS HIERARCHY:
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
#include <limits>
#include <memory>

  // SCL Library

#include "common.h"

namespace SCL
{
  template<typename T,
           typename T_nonconst,
           typename elem_type = typename T::value_type>
  class vector2DvaIterator
  {
  public:
    typedef vector2DvaIterator<T, T_nonconst, elem_type>        self_type;
    typedef T                                                   VECTYPE;
    typedef std::random_access_iterator_tag                     iterator_category;
    typedef typename VECTYPE::value_type                        value_type;
    typedef typename VECTYPE::size_type                         size_type;
    typedef typename VECTYPE::pointer                           pointer;
    typedef typename VECTYPE::const_pointer                     const_pointer;
    typedef typename VECTYPE::reference                         reference;
    typedef typename VECTYPE::const_reference                   const_reference;
    typedef typename VECTYPE::difference_type                   difference_type;


  private:
    VECTYPE *vector2Dva;
    size_t index_;

  protected:
  public:
    friend class vector2DvaIterator<const T, T, const elem_type>;

    vector2DvaIterator(VECTYPE *bt, size_t n) : vector2D(bt), index_(n) {}
    vector2DvaIterator(vector2DvaIterator<T_nonconst, T_nonconst, typename T_nonconst::value_type> const &other)
      : vector2Dva(other.vector2D), index_(other.index_) {}

    elem_type &operator*() { return (*vector2Dva)[index_]; }
    elem_type *operator->() { return *(operator*()); }

    self_type &operator++()
    {
      index_ += 1;
      return *this;
    }

    self_type operator++(int)
    {
      self_type tmp(*this);
      ++(*this);
      return tmp;
    }

    self_type &operator--()
    {
      index_ -= 1;
      return *this;
    }

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
      return index_ == other.index_ && vector2D == other.vector2D;
    }
    bool operator!=(const self_type &other) const
    {
      return index_ != other.index_ && vector2D == other.vector2D;
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

  template <typename vector2DvaIterator_t>
  vector2DvaIterator_t operator+ (const typename vector2DvaIterator_t::difference_type &a, vector2DvaIterator_t const &b)
  {
    return vector2DvaIterator_t(a) + b;
  }

  template <typename vector2DvaIterator_t>
  vector2DvaIterator_t operator- (const typename vector2DvaIterator_t::difference_type &a, vector2DvaIterator_t const &b)
  {
    return vector2DvaIterator_t(a) - b;
  }

  template<class T,
           class Alloc_ = std::allocator<T>>
  class vector2Dva<T>;

  template<typename T>
  class TVectorND
  {
  private:
    T *data;
    vector2Dva<T> *parent;

    bool sameParent(TVectorND *rhs) const { return (rhs->parent == parent); }
    std::size_t columnCount() const { return parent->nColumns; }

  protected:
  public:
    TVectorND(T *d, vector2Dva<T> *p) : data(d), parent(p) {}

    T &operator TVectorND[] (std::size_t n) { return data[n]; }
    T operator TVectorND[] (std::size_type n) const { return data[n]; }




  };

  /// @brief The vector2Dva is an STI-like 2D vector implementation with a fixed column size.
  /// @details This class is used for storing a number of fixed size (maths) vectors. The vector size is fixed when the
  ///          instance is created. This allows some special functionality related to maths vectors to be implemente with minimal
  ///          overhead.

  template<class T,
           class Alloc_ = std::allocator<T>>
  class vector2Dva
  {
  public:
    typedef vector2Dva<T, Alloc_>                                                   self_type;
    typedef Alloc_                                                                  allocator_type;
    typedef typename Alloc_::value_type                                             value_type;
    typedef typename Alloc_::pointer                                                pointer;
    typedef typename Alloc_::const_pointer                                          const_pointer;
    typedef typename Alloc_::reference                                              reference;
    typedef typename Alloc_::const_reference                                        const_reference;
    typedef typename Alloc_::difference_type                                        difference_type;
    typedef typename Alloc_::size_type                                              size_type;
    typedef vector2Dva<self_type, self_type>                                        iterator;
    typedef vector2Dva<const self_type, self_type, const value_type>                const_iterator;
    typedef std::reverse_iterator<iterator>                                         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                                   const_reverse_iterator;

  private:
    allocator_type alloc_;      ///< Allocator to use.
    pointer data_;              ///< Pointer to the data
    size_type nElements;        ///< Actual number of elements stored. Adding elements increases this variable.
    size_type nColumns;         ///< Number of columns, fixed when created.
    size_type nCapacity_;       ///< The allocated capacity of the vector. (number of elements that can be stored.)


    //unsigned int stride(MCL::ENumericType) const;
    //unsigned int stride() const { return stride(dataType); }
    size_t memory() const;
    size_t index(size_t x, size_t y) const { return ( (y * dimX) + x); }

    /// @brief Destroys all allocated elements.
    /// @throws None.
    /// @version 2018-06-02/GGB - Function changed to templated class vector2D.

    void destroyAllElements()
    {
      for (size_type n = 0; n < (nRows * nColumns); ++n)
      {
        alloc_.destroy(bufferArray[indexToSubscript(n)]);
      }
    }

  protected:
  public:
    explicit vector2Dva(size_type , size_type);
    explicit vector2Dva(vector2Dva const &);
    virtual ~vector2Dva();


    pointer data() noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }

    T operator() (size_t, size_t) const;
    T operator() (size_t) const;
    T &operator() (size_t, size_t);
    T &operator() (size_t);

    [[nodiscard]] bool empty() const noexcept { return nRows == 0; }
    size_type size() const noexcept { return nRows; }
    size_type max_size() const noexcept { return (std::numeric_limits<size_type>::max() / nColumns); }
    void reserve(size_type new_cap);
    size_type capacity() const noexcept { return nCapacity_; }
    void shrink_to_fit() {}   // Non binding request... Do nothing.

  };

  /// @brief Constructor for the class.
  /// @param[in] nr - Number of rows for the vector.
  /// @param[in] nc - Number of columns for the vector.
  /// @throws std::bad_alloc
  /// @version 2018-06-06/GGB - Function changed to templated class vector2Dva.
  /// @version 2015-07-07/GGB - Function created.

  template<class T, class Alloc_ = std::allocator<T>>
  vector2Dva::vector2Dva(size_t nr, size_t nc) : data(Alloc_.allocate(nr * nc)), nRows(0), nColumns(nc), nCapacity_(nr)
  {
  }

  /// @brief Destructor for the class.
  /// @details Ensures that the dynamically allocated array is correctly deleted.
  /// @throws None.
  /// @version 2018-06-06/GGB - Function changed to templated class vector2Dva.
  /// @version 2015-07-07/GGB - Function created.

  template<class T, class Alloc_ = std::allocator<T>>
  vector2Dva::~vector2Dva()
  {
    destroyAllElements();
    Alloc_.deallocate(bufferArray, bufferExtent);
  }

  /// @brief Increases the capacity (number of rows) of the vector2D.
  /// @details Increase the capacity of the vector2D to a value that's greater or equal to new_cap. If new_cap is greater than the
  ///          current capacity(), new storage is allocated, otherwise the method does nothing.
  /// @param[in] new_cap: The new capacity to allocate.
  /// @exceptions Any exception throws by Allocator::allocate()
  /// @exceptions std::length_error (if new_cap > max_size()
  /// @note 1. If new_cap is greater than capacity(), all iterators, including the past-the-end iterator, and all references to the
  ///          elements are invalidated. Otherwise, no iterators or references are invalidated.

  template<class T, class Alloc_ = std::allocator<T>>
  void vector2Dva::reserve(size_type new_cap)
  {
    if (new_cap > nCapacity_)
    {
        // Increase the storage and move the elements.
      if (new_cap < max_size())
      {
        pointer newData = Alloc_.allocate(nr * nc);   // May throw.

          // Copy all the elements across. (Only copy allocated elements.

        for (size_type i = 0; i < (nRows * nColumns); i++)
        {
          newData[i] = data_[i];
        };

          // Reassign the pointers.

        Alloc_.deallocate(data_, nCapacity_);
        nCapacity = new_cap;
        data_ = newData;
      }
      else
      {
        throws std::length_error("vector2dva::reserve - Requested size > max_size()");
      }
    };
  }


}	// namespace SCL

#endif // SCL_VECTOR2D_H
