//**********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								array1DP
// SUBSYSTEM:						Polymorphic 2D array
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2020 Gavin Blakeman.
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
// CLASSES INCLUDED:    CArray2DP - Two dimensional vector
//
// CLASS HIERARCHY:
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-08-13 GGB - File Created
//
//**********************************************************************************************************************************

#include "include/array1DP.h"

  // Miscellaneous library header files

#include "boost/locale.hpp"

  // SCL Library

#include "include/algorithm.hpp"

namespace SCL
{

  /// @brief Constructor for the class.
  /// @param[in] nd: New dimension of the array
  /// @throws std::bad_alloc
  /// @note Calls stride() to determine the size of the data elements. Size can throw RUNTIME_ASSERT.
  /// @version 2015-07-07/GGB - Function created.

  CArray1DP::CArray1DP(size_t nd, MCL::ENumericType dt) : data_(nullptr), dim_(nd), dataType(dt)
  {
    data_ = new base_t[dim_ * stride(dataType)];	// Allocate the memory.
  }

  /// @brief Copy constructor.
  /// @throws std::bad_alloc
  /// @note Uses the mult-threaded copy_mt() function.
  /// @version 2011-03-13/GGB - Function created.

  CArray1DP::CArray1DP(const CArray1DP &toCopy) : data_(nullptr), dim_(toCopy.dim_), dataType(toCopy.dataType)
  {
    data_ = new base_t[dim_ * stride(dataType)];

    copy_mt(toCopy.data_, data_, size());
  }

  /// @brief Copy constructor.
  /// @throws std::bad_alloc
  /// @note Uses the mult-threaded copy_mt() function.
  /// @version 2015-08-23/GGB - Function created.

  CArray1DP::CArray1DP(const CArray1DP &toCopy, bool cp) : data_(nullptr), dim_(toCopy.dim_), dataType(toCopy.dataType)
  {
    data_ = new base_t[dim_ * stride(dataType)];

    if (cp)
    {
      copy_mt(toCopy.data_, data_, size());
    };
  }

  /// @brief Destructor for the class.
  /// @throws None.
  /// @details Ensures that the dynamically allocated array is correctly deleted.
  /// @version 2015-07-07/GGB - Function created.

  CArray1DP::~CArray1DP()
  {
    if (data_)
    {
      delete [] data_;
      data_ = nullptr;
    };
  }

  /// @brief      Operator *=
  /// @param      rhs:
  /// @version 2015-08-23/GGB - Function created.

  CArray1DP &CArray1DP::operator*=(CArray1DP const &rhs)
  {
    RUNTIME_ASSERT(dim_ == rhs.dim_, boost::locale::translate("CArray1DP: Invalid container passed in parameter."));

    for(size_t index = 0; index < dim_; index++)
    {
      switch (dataType)
      {
        case MCL::NT_uint8:
        {
          reinterpret_cast<std::uint8_t *>(data_)[index] *= static_cast<std::uint8_t>(rhs.get(index));
          break;
        };
        case MCL::NT_uint16:
        {
          reinterpret_cast<std::uint16_t *>(data_)[index] *= static_cast<std::uint16_t>(rhs.get(index));
          break;
        };
        case MCL::NT_uint32:
        {
          reinterpret_cast<std::uint32_t *>(data_)[index] *= static_cast<std::uint32_t>(rhs.get(index));
          break;
        };
        case MCL::NT_uint64:
        {
          reinterpret_cast<std::uint64_t *>(data_)[index] *= static_cast<std::uint64_t>(rhs.get(index));
          break;
        };
        case MCL::NT_int8:
        {
          reinterpret_cast<std::int8_t *>(data_)[index] *= static_cast<std::int8_t>(rhs.get(index));
          break;
        };
        case MCL::NT_int16:
        {
          reinterpret_cast<std::int16_t *>(data_)[index] *= static_cast<std::int16_t>(rhs.get(index));
          break;
        };
        case MCL::NT_int32:
        {
          reinterpret_cast<std::int32_t *>(data_)[index] *= static_cast<std::int32_t>(rhs.get(index));
          break;
        };
        case MCL::NT_int64:
        {
          reinterpret_cast<std::int64_t *>(data_)[index] *= static_cast<std::int64_t>(rhs.get(index));
          break;
        };
        case MCL::NT_float:
        {
          reinterpret_cast<float *>(data_)[index] *= static_cast<float>(rhs.get(index));
          break;
        };
        case MCL::NT_double:
        {
          reinterpret_cast<double *>(data_)[index] *= static_cast<double>(rhs.get(index));
          break;
        };
        default:
        {
          CODE_ERROR();
        }
      };
    };

    return (*this);
  }

  /// @version 2015-08-23/GGB - Function created.

  CArray1DP &CArray1DP::operator/=(CArray1DP const &rhs)
  {
    RUNTIME_ASSERT(dim_ == rhs.dim_, boost::locale::translate("CArray1DP: Invalid container passed in parameter."));

    for(size_t index = 0; index < dim_; index++)
    {
      switch (dataType)
      {
        case MCL::NT_uint8:
        {
          reinterpret_cast<std::uint8_t *>(data_)[index] /= static_cast<std::uint8_t>(rhs.get(index));
          break;
        };
        case MCL::NT_uint16:
        {
          reinterpret_cast<std::uint16_t *>(data_)[index] /= static_cast<std::uint16_t>(rhs.get(index));
          break;
        };
        case MCL::NT_uint32:
        {
          reinterpret_cast<std::uint32_t *>(data_)[index] /= static_cast<std::uint32_t>(rhs.get(index));
          break;
        };
        case MCL::NT_uint64:
        {
          reinterpret_cast<std::uint64_t *>(data_)[index] /= static_cast<std::uint64_t>(rhs.get(index));
          break;
        };
        case MCL::NT_int8:
        {
          reinterpret_cast<std::int8_t *>(data_)[index] /= static_cast<std::int8_t>(rhs.get(index));
          break;
        };
        case MCL::NT_int16:
        {
          reinterpret_cast<std::int16_t *>(data_)[index] /= static_cast<std::int16_t>(rhs.get(index));
          break;
        };
        case MCL::NT_int32:
        {
          reinterpret_cast<std::int32_t *>(data_)[index] /= static_cast<std::int32_t>(rhs.get(index));
          break;
        };
        case MCL::NT_int64:
        {
          reinterpret_cast<std::int64_t *>(data_)[index] /= static_cast<std::int64_t>(rhs.get(index));
          break;
        };
        case MCL::NT_float:
        {
          reinterpret_cast<float *>(data_)[index] /= static_cast<float>(rhs.get(index));
          break;
        };
        case MCL::NT_double:
        {
          reinterpret_cast<double *>(data_)[index] /= static_cast<double>(rhs.get(index));
          break;
        };
        default:
        {
          CODE_ERROR();
        }
      };
    };

    return (*this);
  }

  /// @brief Subscript operator for class.
  /// @throws 0x0001 - Index out of bounds.
  /// @version 2015-07-09/GGB - Function created.

  MCL::CNumeric CArray1DP::operator() (size_t i) const
  {
    return get(i);
  }

  /// @brief Subscript operator for class.
  /// @throws 0x0001 - Index out of bounds.
  /// @version 2015-07-09/GGB - Function created.

  MCL::CNumeric CArray1DP::operator[] (size_t i) const
  {
    return get(i);
  }

  /// @brief Function to get a stored value from the array.
  /// @param[in] i: Index to return value from.
  /// @throws GCL::CRuntimeAssert

  MCL::CNumeric CArray1DP::get(size_t i) const
  {
    RUNTIME_ASSERT(i < dim_, "Paramter i >= dim_");

    MCL::CNumeric returnValue;

    switch (dataType)
    {
      case MCL::NT_uint8:
      {
        std::uint8_t *array = reinterpret_cast<std::uint8_t *>(data_);
        returnValue = array[i];
        break;
      };
      case MCL::NT_uint16:
      {
        std::uint16_t *array = reinterpret_cast<std::uint16_t *>(data_);
        returnValue = array[i];
        break;
      }
      default:
      {
        CODE_ERROR();
      }

    };

    return returnValue;
  }

  /// @brief Calculate the mean of the data in the array.
  /// @returns The mean value
  /// @throws None
  /// @version 2018-09-20/GGB - Changed return value to a std::optional

  std::optional<MCL::FP_t> CArray1DP::mean() const
  {
    std::optional<MCL::FP_t> returnValue;

    switch (dataType)
    {
      case MCL::NT_uint8:
      {
        returnValue = MCL::mean(static_cast<std::uint8_t *>(data_), size());
        break;
      };
      case MCL::NT_uint16:
      {
        returnValue = MCL::mean(reinterpret_cast<std::uint16_t *>(data_), size());
        break;
      };
      case MCL::NT_uint32:
      {
        returnValue = MCL::mean(reinterpret_cast<std::uint32_t *>(data_), size());
        break;
      };
      case MCL::NT_uint64:
      {
        returnValue = MCL::mean(reinterpret_cast<std::uint64_t *>(data_), size());
        break;
      };
      case MCL::NT_int8:
      {
        returnValue = MCL::mean(reinterpret_cast<std::int8_t *>(data_), size());
        break;
      };
      case MCL::NT_int16:
      {
        returnValue = MCL::mean(reinterpret_cast<std::int16_t *>(data_), size());
        break;
      };
      case MCL::NT_int32:
      {
        returnValue = MCL::mean(reinterpret_cast<std::int32_t *>(data_), size());
        break;
      };
      case MCL::NT_int64:
      {
        returnValue = MCL::mean(reinterpret_cast<std::int64_t *>(data_), size());
        break;
      };
      case MCL::NT_float:
      {
        returnValue = MCL::mean(reinterpret_cast<float *>(data_), size());
        break;
      };
      case MCL::NT_double:
      {
        returnValue = MCL::mean(reinterpret_cast<double *>(data_), size());
        break;
      };
      default:
      {
        CODE_ERROR();
      }
    }

    return returnValue;

  }

  /// @brief Determines the amount of memory (bytes) used by the data array.
  /// @returns The amount of memory occupied by the data array. (Bytes)
  /// @throws None.
  /// @version 2015-07-07/GGB - Function created.

  size_t CArray1DP::memory() const
  {
    return size() * stride();
  }

  /// @version 2015-08-23/GGB - Function created.
  ///

  std::tuple<MCL::FP_t, MCL::FP_t> CArray1DP::minmax() const
  {
    MCL::FP_t min = static_cast<MCL::FP_t>(get(0));
    MCL::FP_t max = static_cast<MCL::FP_t>(get(0));

    for (size_t index = 1; index < dim_; index++)
    {
      min = std::min(min, static_cast<MCL::FP_t>(get(index)));
      max = std::max(max, static_cast<MCL::FP_t>(get(index)));
    };

    return std::make_tuple(min, max);
  }

  /// @brief Converts the data array to the new base type.
  /// @param[in] nt: New base type.
  /// @throws std::bad_alloc
  /// @version 2015-08-23/GGB - Function created.

  void CArray1DP::rebase(MCL::ENumericType nt)
  {
    if (nt != dataType)
    {
      base_t *newData = new base_t[dim_ * stride(nt)];

      for (size_t index = 0; index < dim_; index++)
      {
        switch (nt)
        {
          case MCL::NT_uint8:
          {
            reinterpret_cast<std::uint8_t *>(newData)[index] = static_cast<std::uint8_t>(get(index));
            break;
          };
          case MCL::NT_uint16:
          {
            reinterpret_cast<std::uint16_t *>(newData)[index] = static_cast<std::uint16_t>(get(index));
            break;
          };
          case MCL::NT_uint32:
          {
            reinterpret_cast<std::uint32_t *>(newData)[index] = static_cast<std::uint32_t>(get(index));
            break;
          };
          case MCL::NT_uint64:
          {
            reinterpret_cast<std::uint64_t *>(newData)[index] = static_cast<std::uint64_t>(get(index));
            break;
          };
          case MCL::NT_int8:
          {
            reinterpret_cast<std::int8_t *>(newData)[index] = static_cast<std::int8_t>(get(index));
            break;
          };
          case MCL::NT_int16:
          {
            reinterpret_cast<std::int16_t *>(newData)[index] = static_cast<std::int16_t>(get(index));
            break;
          };
          case MCL::NT_int32:
          {
            reinterpret_cast<std::int32_t *>(newData)[index] = static_cast<std::int32_t>(get(index));
            break;
          };
          case MCL::NT_int64:
          {
            reinterpret_cast<std::int64_t *>(newData)[index] = static_cast<std::int64_t>(get(index));
            break;
          };
          case MCL::NT_float:
          {
            reinterpret_cast<float *>(newData)[index] = static_cast<float>(get(index));
            break;
          };
          case MCL::NT_double:
          {
            reinterpret_cast<double *>(newData)[index] = static_cast<double>(get(index));
            break;
          };
          default:
          {
            CODE_ERROR();
          }
        };
      }

      delete [] data_;
      data_ = newData;
    }
  }

  /// @brief Function to resize the array.
  /// @param[in] nd: New dimension
  /// @param[in] copy: Copy flag. True = copy contents. False = do not copy contents.
  /// @throws std::bad_alloc
  /// @throws GCL::CRuntimeAssert
  /// @details A new array is created, and the information copied into the new array at the same coordinates.
  /// @version 2015-08-13/GGB - Function created.

  void CArray1DP::resize(size_t nd, bool copy)
  {
    base_t *newArray = new base_t[nd * stride(dataType)];     // Can throw std::bad_alloc
    size_t Index;
    size_t iMax;

    iMax = std::min(nd, dim_);

    if (copy)
    {
      for (Index = 0; Index < iMax; Index++)
      {
        newArray[Index] = data_[Index];
      };
    };

    dim_ = nd;
    delete [] data_;
    data_ = newArray;
    newArray = nullptr;
  }

  /// @brief Calculates the stddev of the array.
  /// @version 2018-09-20/GGB - Changed return value to a std::optional.

  std::optional<MCL::FP_t> CArray1DP::stdev() const
  {
    std::optional<MCL::FP_t> returnValue;

    return returnValue;
  }


  /// @brief Returns the stride value of the array.
  /// @returns The stride value. (sizeof the individual elements)
  /// @details The stride value is determined on the underlying data type.
  /// @throws GCL::CRuntimeAssert
  /// @version 2015-08-13/GGB - Function created.

  unsigned int CArray1DP::stride(MCL::ENumericType dt) const
  {
    unsigned int returnValue = 0;

    switch (dt)
    {
      case MCL::NT_none:
      {
        CODE_ERROR();
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
        CODE_ERROR();
        break;
      };
    };

    return returnValue;
  }

}   // namespace SCL
