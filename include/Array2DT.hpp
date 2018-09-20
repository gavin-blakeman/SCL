//*************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								CArray2DT
// SUBSYSTEM:						Templated classes for image arrays
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	SCL
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2011-2018 Gavin Blakeman.
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
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//
//*************************************************************************************************

#ifndef SCL_ARRAY2DT_H
#define SCL_ARRAY2DT_H

#include "Base.h"
#include "common.h"

#include <algorithm>

#include <GCL>

#ifdef _MSC_VER
#pragma warning( disable : 4290 )  /* Disable the warning about C++ exception specifications. */
#endif

namespace SCL
{
  /// @brief The CArray2DT array is an encapsulated 2-dimensional array.
  /// @details The class is used for storing two dimensional arrays.

  template<typename T>
  class CArray2DT : public CContainer
  {
  private:
    T *imageArray;
    size_t xDim, yDim;

  protected:
  public:
    explicit CArray2DT(size_t , size_t);
    explicit CArray2DT(const CArray2DT<T> &);
    virtual ~CArray2DT();

    const T & operator[] (size_t) const;
    T & operator[] (size_t);

    const T & operator() (size_t, size_t) const;
    T & operator() (size_t, size_t);

    size_t x() const {return xDim;}
    size_t y() const {return yDim;}

    virtual void resize(size_t, size_t, bool = false);
  };

  template<typename T>
  CArray2DT<T>::CArray2DT(size_t newX, size_t newY) : xDim(newX), yDim(newY), imageArray(nullptr)
  {
    imageArray = new T[xDim * yDim];	// Allocate the memory.
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
  /// @param[in] index - The index of the desired object.
  /// @returns The object at the desired index.
  /// @throws SCL_ERROR(0x0001)
  /// @throws SCL_ERROR(0x0002)
  /// @version 2017-06-25/GGB - Updated to use new GCL error handling.
  /// @version 2011-03-13/GGB - Function created.

  template<typename T>
  const T & CArray2DT<T>::operator[] (size_t index) const
  {
    if (imageArray)
    {
      if (index >= (xDim * yDim))
      {
        SCL_ERROR(0x0001);
      }
      else
      {
        return imageArray[index];
      }
    }
    else
    {
      SCL_ERROR(0x0002);
    }
  }

  /// @brief Subscript operator for class.
  /// @param[in] index - The index of the desired object.
  /// @returns The object at the desired index.
  /// @throws SCL_ERROR(0x0001)
  /// @throws SCL_ERROR(0x0002)
  /// @version 2017-06-25/GGB - Updated to use new GCL error handling.
  /// @version 2011-03-13/GGB - Function created.

  template<typename T>
  T & CArray2DT<T>::operator[] (size_t index)
  {
    if (index >= (xDim * yDim))
    {
      SCL_ERROR(0x0001);
    }
    else
    {
      return imageArray[index];
    }
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
