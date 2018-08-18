//*************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								valarray2DP
// SUBSYSTEM:						Polymorphic valarray
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CArray2DT - Two dimensional array
//
// CLASS HIERARCHY:
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-07-07 GGB - File Created
//
//*************************************************************************************************

#ifndef SCL_ARRAYT_H
#define SCL_ARRAYT_H

  // Standard libraries

#include <algorithm>
#include <cstdint>

  // SCL Library

#include "Base.h"

  // MCL Library

#include "../MCL/Include/numeric.h"

// Boost library

#include "boost/optional.hpp"

namespace SCL
{
  /// @brief The CValarray2DP array is an encapsulated 2-dimensional array dynamic array.
  /// @details The class is used for storing two dimensional arrays.

  class CValarray2DP
  {
    typedef std::uint8_t base_t;

  private:
    base_t *data;
    size_t dimX, dimY;
    MCL::ENumericType dataType;

    unsigned int stride(MCL::ENumericType) const;
    unsigned int stride() const { return stride(dataType); }
    size_t memory() const;
    size_t index(size_t x, size_t y) const { return ( (y * dimX) + x); }

  protected:
  public:
    explicit CValarray2DP(size_t , size_t, MCL::ENumericType);
    explicit CValarray2DP(const CValarray2DP &);
    virtual ~CValarray2DP();

    size_t size() const { return dimX * dimY; }

    MCL::CNumeric operator() (size_t, size_t) const;
    MCL::CNumeric & operator() (size_t, size_t);

    size_t x() const { return dimX; }
    size_t y() const { return dimY; }

    virtual void resize(size_t, size_t, bool = false);

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

}	// namespace SCL

#endif // SCL_ARRAYT_H
