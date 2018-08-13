//**********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								vector2D
// SUBSYSTEM:						2D vector
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	SCL
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2018 Gavin Blakeman.
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
#include <memory>

  // SCL Library

#include "common.h"

  // MCL Library

#include "../MCL/Include/numeric.h"

  // Boost library

#include "boost/optional.hpp"

namespace SCL
{
  template<typename T,
           typename T_nonconst,
           typename elem_type = typename T::value_type>
  class vector2DIterator
  {
  public:
    typedef vector2DIterator<T, T_nonconst, elem_type>          self_type;
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
    VECTYPE *vector2D;
    size_t index_;

  protected:
  public:
    friend class vector2DIterator<const T, T, const elem_type>;

    vector2DIterator(VECTYPE *bt, size_t n) : vector2D(bt), index_(n) {}
    vector2DIterator(vector2DIterator<T_nonconst, T_nonconst, typename T_nonconst::value_type> const &other)
      : vector2D(other.vector2D), index_(other.index_) {}

    elem_type &operator*() { return (*vector2D)[index_]; }
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

  template <typename vector2DIterator_t>
  vector2DIterator_t operator+ (const typename vector2DIterator_t::difference_type &a, vector2DIterator_t const &b)
  {
    return vector2DIterator_t(a) + b;
  }

  template <typename vector2DIterator_t>
  vector2DIterator_t operator- (const typename vector2DIterator_t::difference_type &a, vector2DIterator_t const &b)
  {
    return vector2DIterator_t(a) - b;
  }

  /// @brief The vector2D is an STI-like 2D vector implementation.
  /// @details The class is used for storing two dimensional vectors.

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
    typedef vector2D<self_type, self_type>                                          iterator;
    typedef vector2D<const self_type, self_type, const value_type>                  const_iterator;
    typedef std::reverse_iterator<iterator>                                         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                                   const_reverse_iterator;

  private:
    //allocator_type alloc_;                                ///< Allocator to use.
    pointer data;                                         ///< Pointer to the data
    size_type nRows;                                      ///< Number of rows. Adding elements expands the number of rows.
    size_type nColumns;                                   ///< Number of columns.


    unsigned int stride(MCL::ENumericType) const;
    unsigned int stride() const { return stride(dataType); }
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
    explicit vector2D(size_type , size_type);
    explicit vector2D(const CArray2DP &);
    virtual ~vector2D();

    size_t size() const { return dimX * dimY; }

    MCL::CNumeric operator() (size_t, size_t) const;
    MCL::CNumeric operator() (size_t) const;
    MCL::CNumeric &operator() (size_t, size_t);
    MCL::CNumeric &operator() (size_t);

    size_t x() const { return dimX; }
    size_t y() const { return dimY; }

    virtual void resize(size_t, size_t, bool = false);

    boost::optional<MCL::FP_t> mean() const;
  };

  /// @brief Constructor for the class.
  /// @param[in] nr - Number of rows for the vector.
  /// @param[in] nc - Number of columns for the vector.
  /// @throws std::bad_alloc
  /// @version 2018-06-02/GGB - Function changed to templated class vector2D.
  /// @version 2015-07-07/GGB - Function created.

  template<class T, class Alloc_ = std::allocator<T>>
  vector2D::vector2D(size_t nr, size_t nc) : data(Alloc_.allocate(nr *nc)), nRows(nr), nColumns(nc)
  {
  }

  /// @brief Destructor for the class.
  /// @details Ensures that the dynamically allocated array is correctly deleted.
  /// @throws None.
  /// @version 2018-06-02/GGB - Function changed to templated class vector2D.
  /// @version 2015-07-07/GGB - Function created.

  template<class T, class Alloc_ = std::allocator<T>>
  vector2D::~vector2D()
  {
    destroyAllElements();
    Alloc_.deallocate(bufferArray, bufferExtent);
  }


}	// namespace SCL

#endif // SCL_VECTOR2D_H
