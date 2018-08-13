//**********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								valarray_r
// SUBSYSTEM:						reference valarray
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CArray2DT - Two dimensional array
//
// CLASS HIERARCHY:
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-07-07 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef SCL_ARRAYT_H
#define SCL_ARRAYT_H

  // Standard libraries

#include <algorithm>
#include <cstdint>

  // Boost library

#include "boost/optional.hpp"

namespace SCL
{
  /// @brief The valarray_r class is a STL type class that uses referenced arrays and only copies data if required.
  /// @details

  template<typename T,
           class Alloc_ = std::allocator<T>>
  class valarray_r
  {
    static_assert(std::is_arithmetic<T>::value, "typename T must be numeric");

  public:
    typedef valarray_r<T, Alloc_>                                                   self_type;
    typedef Alloc_                                                                  allocator_type;
    typedef typename Alloc_::value_type                                             value_type;
    typedef typename Alloc_::pointer                                                pointer;
    typedef typename Alloc_::const_pointer                                          const_pointer;
    typedef typename Alloc_::reference                                              reference;
    typedef typename Alloc_::const_reference                                        const_reference;
    typedef typename Alloc_::difference_type                                        difference_type;
    typedef typename Alloc_::size_type                                              size_type;
    typedef valarray_r<self_type, self_type>                                        iterator;
    typedef valarray_r<const self_type, self_type, const value_type>                const_iterator;
    typedef std::reverse_iterator<iterator>                                         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                                   const_reverse_iterator;

  private:
    pointer data_;
    size_type dim;
    bool ownsData;

    size_type memory() const;

  protected:
    explicit valarray_r() = delete;           // Not allowed to have empty default initialisation

  public:
    explicit valarray_r(valarray_r const &source);
    explicit valarry_r(pointer data, size_type dim) : data_(data), dim_(dim), ownsData(false) {}
    virtual ~valarray_r();

    size_type size() const { return dim_; }

    T operator() (size_type index) const { return data[index]; }
    T &operator() (size_type);


    boost::optional<MCL::FP_t> mean() const;
  };

  /// @brief Function to resize the array. A new array is created, and the information copied into the
  /// new array at the same coordinates.
  //
  // 2011-03-13/GGB - Function created.

/*  template<typename T>
  void CValarray2DP<T>::resize(size_t newx, size_t newy, bool copy)
  {
    T* newImageArray = new T[newx * newy];
    size_t xIndex, yIndex;
    size_t xMax, yMax;

    xMax = std::min(newx, xDim);
    yMax = std::min(newy, yDim);

    if (copy)
    {
      for (yIndex = 0; yIndex < yMax; yIndex++)
        for (xIndex = 0; xIndex < xMax; xIndex++)
          newImageArray[xIndex + (yIndex * newy)] = imageArray[xIndex + (yIndex * yDim)];
    };

    xDim = newx;
    yDim = newy;
    delete [] imageArray;
    imageArray = newImageArray;
    newImageArray = nullptr;
  } */


  /// @brief Class destructor.
  /// @throws None.
  /// @version 2018-06-18/GGB - Function created.

  template<class T,
           class Alloc_ = std::allocator<T>>
  valarray_r<T, Alloc_>::~valarray_r()
  {
    if (ownsData)
    {
      Alloc_.deallocate(data_, dim_);
      data_ = nullptr;
    }
    else
    {
      data_ = nullptr;
    }
  }

}	// namespace SCL

#endif // SCL_ARRAYT_H
