//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								array2D
// SUBSYSTEM:						Templated classes for image arrays
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2011-2020 Gavin Blakeman.
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
// OVERVIEW:            An STL style two dimensional array that can be sized at runtime. The array cannot be resized once it has
//                      been initialised.
//                      The type of the array must be default constructable.
//
// CLASSES INCLUDED:    array2D - Two dimensional templated array
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//
//**********************************************************************************************************************************

#ifndef SCL_ARRAY2DT_H
#define SCL_ARRAY2DT_H

  // Standard C++ library header files.

#include <algorithm>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <utility>

  // Miscellaneous library header files.

//#include <GCL>

  // SCL Library header files.

#include "common.h"
#include "config.h"
#include "error.h"

#ifdef _MSC_VER
#pragma warning( disable : 4290 )  /* Disable the warning about C++ exception specifications. */
#endif

namespace SCL
{

  template<typename T>
  class array2DIndex
  {
  private:
    T row_;
    T column_;

  public:
    array2DIndex() : row_(0), column_(0) {}
    array2DIndex(T row, T column) : row_(row), column_(column) {}
    array2DIndex(array2DIndex const &toCopy) : row_(toCopy.row_), column_(toCopy.column_) {}

    array2DIndex &operator=(array2DIndex const &lhs)
    {
      if (lhs != *this)
      {
        row_ = lhs.row_;
        column_ = lhs.column_;
      };
      return *this;
    }

    bool operator==(array2DIndex const &lhs) { return ( (row_ == lhs.row_) && (column_ == lhs.column_)); }

    T row() const noexcept { return row_; }
    T column() const noexcept { return column_; }
  };

  /// @class A templated 2-dimensional vector type container. The container is always rectangular.
  /// @tparam T: Type to use as the data to store in the hierarchy.
  /// @note 1: Reallocation can be an expensive operation as the copy operation needs to be operated with reference to each
  ///          row. This is multi-threaded if the SCL_THREAD macro is defined.

  template<typename T,
           class Alloc_ = std::allocator<T>>
  class array2D
  {
  public:
//    typedef array2D<T, Alloc_>                                              self_type;
//    typedef T                                                               elem_type;
    typedef Alloc_                                                          allocator_type;
//    typedef typename Alloc_::value_type                                     value_type;
    using pointer = std::allocator_traits<Alloc_>::pointer;
//    typedef typename std::allocator_traits<allocator_type>::const_pointer   const_pointer;
//    typedef T&                                                              reference;
//    typedef typename Alloc_::const_reference                                const_reference;
//    typedef typename Alloc_::difference_type                                difference_type;
    typedef typename Alloc_::size_type                                      size_type;

  private:
    Alloc_ alloc_;
    size_type xExtent_, yExtent_;         ///< Number of rows and columns in the 2D vector.
    pointer dataStorage_;                 ///< The underlying storage.

#ifdef SCL_THREAD
    std::mutex classMutex_;
#endif

    inline size_type indexToSubscript(size_type x, size_type y) const noexcept { return (x + y * xExtent_); }
    inline size_type arrayExtent() const noexcept { return xExtent_ * yExtent_; }


    /// @brief Copies elements from one container to another container.
    /// @tparam f_iter: The iterator type.
    /// @param[in] from: Iterator to copy from.
    /// @param[in] to: Iterator to copy to.
    /// @throws None.
    /// @version 2019-11-30/GGB - Function created.

    template<typename f_iter>
    void assignInto(f_iter from, f_iter to)
    {
      while (from != to)
      {
        push_back(*from);
        ++from;
      };
    }

    /// @brief Destroys all allocated elements.
    /// @throws None.
    /// @version 2019-11-30/GGB - Function created.

    void destroyAllElements()
    {
      size_type const elementCount = arrayExtent();

      std::destroy_n(dataStorage_, elementCount);

      alloc_.deallocate(dataStorage_, elementCount);
    }

  public:

    /// @brief Initialiser constructor
    /// @param[in] xExtent: The number of x elements to allocate (rows)
    /// @param[in] yExtent: The number of y elements to allocate (columns)
    /// @throws std::bad_alloc
    /// @throws
    /// @version 2020-02-15/GGB - Function created.

    explicit array2D(size_type xExtent, size_type yExtent) : alloc_(), xExtent_(xExtent), yExtent_(yExtent),
      dataStorage_(alloc_.allocate(arrayExtent()))
#ifdef SCL_THREAD
  , classMutex_()
#endif
    {
      for (size_type i = 0; i < arrayExtent(); ++i)
      {
        dataStorage_[i] = T();
      };
    }

    /// @brief Initialiser constructor
    /// @param[in] xExtent: The number of x elements to allocate (rows)
    /// @param[in] yExtent: The number of y elements to allocate (columns)
    /// @param[in] value: The value to initialise.
    /// @param[in] alloc: The allocator to utilise.
    /// @throws std::bad_alloc
    /// @throws
    /// @version 2019-12-08/GGB - Function created.

    explicit array2D(size_type xExtent, size_type yExtent, T const &value, Alloc_ const &alloc = Alloc_())
    : alloc_(alloc), xExtent_(xExtent), yExtent_(yExtent), dataStorage_(alloc_.allocate(arrayExtent()))
#ifdef SCL_THREAD
  , classMutex_()
#endif
    {
      for (size_type i = 0; i < arrayExtent(); ++i)
      {
        dataStorage_[i] = value;
      };
    }

    /// @brief Copy constructor.
    /// @param[in] toCopy: The instance to create a copy of.
    /// @version 2019-11-30/GGB - Converted to STL type container.
    /// @version 2011-03-13/GGB - Function created.

    explicit array2D(array2D const &toCopy) : dataStorage_(alloc_.allocate(toCopy.xExtent_ * toCopy.yExtent_)),
      xExtent_(toCopy.xExtent_), yExtent_(toCopy.yExtent_)
#ifdef SCL_THREAD
    , classMutex_()
#endif
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif

      try
      {
        assignInto(toCopy.begin(), toCopy.end());
      }
      catch(...)
      {
        destroyAllElements();
        alloc_.deallocate(dataStorage_, toCopy.xExtent_ * toCopy.yExtent_);
        throw;
      };
    }


    /// @brief Class destructor.
    /// @throws None.
    /// @version 2019-11-30/GGB - Converted to STL type container.
    /// @version 2011-03-13/GGB - Function created.

    virtual ~array2D()
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      destroyAllElements();
      alloc_.deallocate(dataStorage_, arrayExtent());
    }

    /// @brief Subscript operator for class.
    /// @param[in] row: The row number of the element
    /// @param[in] column: The column number of the element.
    /// @throws std::out_of_range: If the subscript is out of range.
    /// @note 1. Performs range checking. IE does the value fall within the defined range.
    /// @version 2019-11-30/GGB - Updated to STL style container.
    /// @version 2017-06-25/GGB - Updated to use new GCL error handling.
    /// @version 2011-03-13/GGB - Function created.

    T const &operator()(size_type row, size_type col) const
    {
      if ( (row < xExtent_) && (col < yExtent_) )
      {
        return dataStorage_[indexToSubscript(row , col)];
      }
      else
      {
        throw std::out_of_range("index requested is out of range.");
      };
    }


    /// @brief Subscript operator for class.
    /// @param[in] row: The row number of the element
    /// @param[in] column: The column number of the element.
    /// @throws std::out_of_range
    /// @note 1. Performs range checking. IE does the value fall within the defined range.
    /// @version 2019-11-30/GGB - Updated to STL style container.
    /// @version 2017-06-25/GGB - Updated to use new GCL error handling.
    /// @version 2011-03-13/GGB - Function created.

    T &operator()(size_type row, size_type col)
    {
      if ( (row < xExtent_) && (col < yExtent_) )
      {
        return dataStorage_[indexToSubscript(row, col)];
      }
      else
      {
        throw std::out_of_range("index requested is out of range.");
      };
    }

    /// @brief Returns the element at the corresponding position.
    /// @param[in] row: The row number of the element
    /// @param[in] column: The column number of the element.
    /// @throws std::out_of_range
    /// @version 2019-12-03/GGB - Function created.

    T &at(size_type row, size_type column)
    {
      return (*this)(row, column);
    }

    /// @brief Returns the element at the corresponding position.
    /// @param[in] row: The row number of the element
    /// @param[in] column: The column number of the element.
    /// @throws std::out_of_range
    /// @version 2019-12-03/GGB - Function created.

    T const &at(size_type row, size_type column) const
    {
      return (*this)(row, column);
    }

    /// @brief Tests if the vector is empty.
    /// @throws None.
    /// @threadsafe
    /// @version 2019-12-01/GGB - Function created.

    bool empty() const noexcept
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      return (xExtent_ == 0) || (yExtent_ == 0);
    }

    /// @brief Returns the size of the array.
    /// @returns The size of the array.
    /// @throws None.
    /// @version 2019-11-30/GGB - Function created.

    std::pair<size_type, size_type> size() const noexcept
    {
#ifdef SCL_THREAD
      std::lock_guard<std::mutex> lg(classMutex_);
#endif
      return std::make_pair(xExtent_, yExtent_);
    }

    /// @brief Returns the maximum number of elements the array can hold. As the array is a fixed size container once created,
    ///        this returns the same value as size()
    /// @throws None.
    /// @version 2019-12-03/GGB - Function created.

    std::pair<size_type, size_type> max_size() const noexcept
    {
      return size();
    }


  };    // Class array2D


}	// namespace SCL


#endif // SCL_ARRAY2DT_H
