//**********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								vectorStatic
// SUBSYSTEM:						Fixed size low-overhead vector.
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
// OVERVIEW:            This file implements a vector of fixed size that has a lightweight interface to minimise storage space.
//
// CLASSES INCLUDED:    SCL::vector_static<>
//
// CLASS HIERARCHY:
//
// HISTORY:             2018-06-01 GGB - File created.
//
//**********************************************************************************************************************************

#ifndef SCL_ARRAY2DT_H
#define SCL_ARRAY2DT_H

#include "common.h"

#include <algorithm>

#include <GCL>

#ifdef _MSC_VER
#pragma warning( disable : 4290 )  /* Disable the warning about C++ exception specifications. */
#endif

namespace SCL
{
  /// @brief This implements a static vector. IE a fixed size vector.
  /// @details This class is used for storing a static vector. This is a fixed length vector. The length of the vector cannot be
  ///          changed after the class is created.


  template<typename T,
           size_t vectorSize_,
           class Alloc_ = std::allocator<T>>
  class vector_static
  {
  public:
    typedef vector_static<T, vectorSize_, Alloc_>                                   self_type;
    typedef Alloc_                                                                  allocator_type;
    typedef typename Alloc_::value_type                                             value_type;
    typedef typename Alloc_::pointer                                                pointer;
    typedef typename Alloc_::const_pointer                                          const_pointer;
    typedef typename Alloc_::reference                                              reference;
    typedef typename Alloc_::const_reference                                        const_reference;
    typedef typename Alloc_::difference_type                                        difference_type;
    typedef typename Alloc_::size_type                                              size_type;
    //typedef TCircularBufferIterator<self_type, self_type>                           iterator;
    //typedef TCircularBufferIterator<const self_type, self_type, const value_type>   const_iterator;
    typedef std::reverse_iterator<iterator>                                         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                                   const_reverse_iterator;

  private:
    T *storageArray;

    allocator_type alloc_;
    pointer bufferArray;                    ///< Memory array for the buffer contents.
    size_type elementCount;                 ///< The number of elements contained.

  protected:
  public:
    explicit vector_static();
    explicit vector_static(vector_static<T> const &);
    explicit vector_static(T *);

    virtual ~vector_static();

    /// @{
    /// @brief Returns a reference to the element at specified location pos.
    /// @param[in] pos - position of the element to return.
    /// @returns Reference to the requested element.
    /// @throws std::out_of_range
    /// @note 1. Bounds checking is performed.

    reference at(size_type pos) { if (pos < vectorSize_) {return storageArray[pos]; } else { throw std::out_of_range; }; }
    const_reference at(size_type pos) const { if (pos < vectorSize_) {return storageArray[pos]; } else { throw std::out_of_range; }; }

    /// @}

    /// @{
    /// @brief Returns a reference to the element at specified location pos.
    /// @param[in] pos - position of the element to return.
    /// @returns Reference to the requested element.
    /// @throws none.
    /// @note 1. No bounds checking is performed.

    reference operator[] (size_type pos) { return storageArray[pos]; }
    const_reference operator[] (size_type) const { return storageArray[pos]; }

    /// @}

    /// @{
    /// @brief Returns a reference to the element at specified location pos.
    /// @returns Pointer to the underlying array serving as element storage. The pointer is such that range[data(); data() + size()]
    ///          is always a valid range.
    /// @throws none.
    /// @note 1. If size() == 0, data() will return a nullptr.

    T *data() noexcept { return storageArray; }
    const T* data() const noexcept { return storageArray; }

    /// @}

    const T & operator() (size_t, size_t) const;
    T & operator() (size_t, size_t);

    size_type size()const noexcept { return vectorSize_; }
    size_type capacity() const noexcept { return vectorSize_; }
    size_type max_size() const noexcept { return vectorSize_; }
    bool empty() const noexcept { return false; }

    virtual void resize(size_t, size_t, bool = false);
  };

  /// @brief Default constuctor for the class.
  /// @throws None.
  /// @version 2018-06-01/GGB - Function created.

  template<typename T,
           size_t vectorSize_,
           class Alloc_ = std::allocator<T>>
  vector_static<T, vectorSize, std::allocator<T>>::vector_static()
  {

  }

  /// @brief Copy constructor.
  /// @param[in] toCopy - The instance to create a copy of.
  /// @version 2011-03-13/GGB - Function created.

  template<typename T>
  CArray2DT<T>::CArray2DT(const CArray2DT<T> &toCopy) : xDim(toCopy.xDim), yDim(toCopy.yDim), imageArray(NULL)
  {
    size_t index = 0;

    imageArray = new T[xDim * yDim];

    for (; index < (xDim * yDim); index++)
    {
      imageArray[index] = toCopy.imageArray[index];
    }
  }

  /// @brief Destructor for the class.
  /// @details Ensures that the dynamically allocated array is correctly deleted.
  /// @throws None.
  /// @version 2011-03-13/GGB - Function created.

  template<typename T>
  CArray2DT<T>::~CArray2DT()
  {
    if (imageArray)
    {
      delete imageArray;
      imageArray = nullptr;
    };
  }



  /// @brief Subscript operator for class.
  /// @version 2017-06-25/GGB - Updated to use new GCL error handling.
  // 2011-03-13/GGB - Function created.

  template<typename T>
  const T & CArray2DT<T>::operator() (unsigned long x, unsigned long y) const
  {
    if ( (x > xDim) || (y > yDim) )
    {
      SCL_ERROR(0x0001);
    }
    else
    {
      return imageArray[x + (yDim * y)];
    }
  }

  /// @brief Subscript operator for class.
  /// @version 2017-06-25/GGB - Updated to use new GCL error handling.
  // 2011-03-13/GGB - Function created.

  template<typename T>
  T & CArray2DT<T>::operator() (unsigned long x, unsigned long y)
  {
    if ( (x > xDim) || (y > yDim) )
    {
      SCL_ERROR(0x0001);
    }
    else
    {
      return imageArray[x + (yDim * y)];
    }
  }

  /// @brief Function to resize the array. A new array is created, and the information copied into the
  /// new array at the same coordinates.
  //
  // 2011-03-13/GGB - Function created.

  template<typename T>
  void CArray2DT<T>::resize(size_t newx, size_t newy, bool copy)
  {
    T* newImageArray = new T[newx * newy];
    size_t xIndex, yIndex;
    size_t xMax, yMax;

    xMax = std::min(newx, xDim);
    yMax = std::min(newy, yDim);

    if (copy)
    {
      for (yIndex = 0; yIndex < yMax; yIndex++)
      {
        for (xIndex = 0; xIndex < xMax; xIndex++)
        {
          newImageArray[xIndex + (yIndex * newy)] = imageArray[xIndex + (yIndex * yDim)];
        };
      };
    };

    xDim = newx;
    yDim = newy;
    delete [] imageArray;
    imageArray = newImageArray;
    newImageArray = nullptr;
  }

}	// namespace SCL

#endif // SCL_ARRAY2DT_H


