//*************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								valarray2DP
// SUBSYSTEM:						Polymorphic valarray
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2018-2019 Gavin Blakeman.
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

#include "../include/valarray2DP.h"

  // Miscellaneous libraries

#include <MCL>

  // SCL Library

#include "../include/algorithm.hpp"

namespace SCL
{

  /// @brief Constructor for the class.
  /// @param[in] nx: New x-dimension of the array
  /// @param[in] ny: New y-dimension of the array
  /// @throws std::bad_alloc
  /// @note Calls stride() to determine the size of the data elements. Size can throw RUNTIME_ASSERT.
  /// @version 2015-07-07/GGB - Function created.

  CValarray2DP::CValarray2DP(size_t nx, size_t ny, MCL::ENumericType dt) : data(nullptr), dimX(nx), dimY(ny), dataType(dt)
  {
    data = new base_t[dimX * dimY * stride(dataType)];	// Allocate the memory.
  }

  /// @brief Copy constructor.
  /// @throws std::bad_alloc
  /// @note Uses the mult-threaded copy_mt() function.
  /// @version 2011-03-13/GGB - Function created.

  CValarray2DP::CValarray2DP(const CValarray2DP &toCopy) : data(nullptr), dimX(toCopy.dimX), dimY(toCopy.dimY),
    dataType(toCopy.dataType)
  {
    data = new base_t[dimX * dimY * stride(dataType)];

    copy_mt(toCopy.data, data, size());
  }

  /// @brief Destructor for the class.
  /// @details Ensures that the dynamically allocated array is correctly deleted.
  /// @throws None.
  /// @version 2015-07-07/GGB - Function created.

  CValarray2DP::~CValarray2DP()
  {
    if (data)
    {
      delete [] data;
      data = nullptr;
    };
  }

  /// @brief Subscript operator for class.
  /// @throws 0x0001 - Index out of bounds.
  /// @version 2015-07-09/GGB - Function created.

  MCL::CNumeric CValarray2DP::operator() (size_t x, size_t y) const
  {
    MCL::CNumeric returnValue;

    if ( (x > dimX) || (y > dimY) )
    {
      SCL_ERROR(0x0001);
    }
    else
    {
      switch (dataType)
      {
        case MCL::NT_uint8:
        {
          std::uint8_t *array = static_cast<std::uint8_t *>(data);
          returnValue = array[index(x, y)];
          break;
        };
        case MCL::NT_uint16:
        {
          std::uint16_t *array = reinterpret_cast<std::uint16_t *>(data);
          returnValue = array[index(x, y)];
          break;
        }
      };
    };

    return returnValue;
  }

  /// @brief Calculate the mean of the data in the array.
  //

  std::optional<MCL::FP_t> CValarray2DP::mean() const
  {
    std::optional<MCL::FP_t> returnValue;

    switch (dataType)
    {
      case MCL::NT_uint8:
      {
        //returnValue = MCL::mean(static_cast<std::uint8_t *>(data), size());
        break;
      };
      case MCL::NT_uint16:
      {
        //returnValue = MCL::mean(reinterpret_cast<std::uint16_t *>(data), size());
        break;
      };
      case MCL::NT_uint32:
      {
        //returnValue = MCL::mean(reinterpret_cast<std::uint32_t *>(data), size());
        break;
      };
      case MCL::NT_uint64:
      {
        //returnValue = MCL::mean(reinterpret_cast<std::uint64_t *>(data), size());
        break;
      };
      case MCL::NT_int8:
      {
        //returnValue = MCL::mean(reinterpret_cast<std::int8_t *>(data), size());
        break;
      };
      case MCL::NT_int16:
      {
        //returnValue = MCL::mean(reinterpret_cast<std::int16_t *>(data), size());
        break;
      };
      case MCL::NT_int32:
      {
        //returnValue = MCL::mean(reinterpret_cast<std::int32_t *>(data), size());
        break;
      };
      case MCL::NT_int64:
      {
        //returnValue = MCL::mean(reinterpret_cast<std::int64_t *>(data), size());
        break;
      };
      case MCL::NT_float:
      {
        //returnValue = MCL::mean(reinterpret_cast<float *>(data), size());
        break;
      };
      case MCL::NT_double:
      {
        //returnValue = MCL::mean(reinterpret_cast<double *>(data), size());
        break;
      };
    }

    return returnValue;

  }

  /// @brief Determines the amount of memory (bytes) used by the data array.
  /// @returns The amount of memory occupied by the data array. (Bytes)
  /// @throws None.
  //
  // 2015-07-07/GGB - Function created.

  size_t CValarray2DP::memory() const
  {
    return size() * stride();
  }


  /*! @brief Returns the stride value of the array.
   *  @details The stride value is determined on the underlying data type.
   * @returns The stride value. (sizeof the individual elements)
   * @throws RUNTIME_ASSERT.
   */

  unsigned int CValarray2DP::stride(MCL::ENumericType dt) const
  {
    unsigned int returnValue = 0;

    switch (dt)
    {
      case MCL::NT_none:
      {
        CODE_ERROR;
        break;
      };
      case MCL::NT_uint8:
      {
        returnValue = sizeof(std::uint8_t);
        break;
      };
      case MCL::NT_uint16:
      {
        returnValue = sizeof(std::uint16_t);
        break;
      };
      case MCL::NT_uint32:
      {
        returnValue = sizeof(std::uint32_t);
        break;
      };
      case MCL::NT_uint64:
      {
        returnValue = sizeof(std::uint64_t);
        break;
      };
      case MCL::NT_int8:
      {
        returnValue = sizeof(std::int8_t);
        break;
      };
      case MCL::NT_int16:
      {
        returnValue = sizeof(std::int16_t);
        break;
      };
      case MCL::NT_int32:
      {
        returnValue = sizeof(std::int32_t);
        break;
      };
      case MCL::NT_int64:
      {
        returnValue = sizeof(std::int64_t);
        break;
      };
      case MCL::NT_float:
      {
        returnValue = sizeof(float);
        break;
      };
      case MCL::NT_double:
      {
        returnValue = sizeof(double);
        break;
      };
      default:
      {
        CODE_ERROR;
        break;
      };
    };

    return returnValue;
  }

}   // namespace SCL
