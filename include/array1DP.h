//**********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								array1DP
// SUBSYSTEM:						Polymorphic 1D array
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2018-2020 Gavin Blakeman.
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
// OVERVIEW:            Provides a one dimensional array with polymorphic numerical performance.
//
// CLASSES INCLUDED:    CArray1DP - Two dimensional vector
//
// CLASS HIERARCHY:
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-08-13 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef SCL_ARRAY1DP_H
#define SCL_ARRAY1DP_H

  // Standard libraries

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

  // Miscellaneous Libraries

#include <MCL>

  // SCL Library

#include "Base.h"
#include "common.h"
#include "error.h"

namespace SCL
{
  /// @brief The CArray1DP array is an encapsulated 2-dimensional array dynamic array.
  /// @details The class is used for storing two dimensional arrays.

  class CArray1DP
  {
    typedef std::uint8_t base_t;

  private:
    base_t *data_;
    size_t dim_;
    MCL::ENumericType dataType;

    unsigned int stride(MCL::ENumericType) const;
    unsigned int stride() const { return stride(dataType); }
    size_t memory() const;

  protected:
  public:
    explicit CArray1DP(size_t, MCL::ENumericType);
    explicit CArray1DP(CArray1DP const &);
    CArray1DP(CArray1DP const &, bool);
    virtual ~CArray1DP();

    size_t size() const { return dim_; }
    base_t *data() { return data_; }

    MCL::CNumeric operator()(size_t) const;
    MCL::CNumeric operator[](size_t) const;

    CArray1DP &operator*=(CArray1DP const &);
    CArray1DP &operator/=(CArray1DP const &);
    CArray1DP &operator%=(CArray1DP const &);

    MCL::CNumeric get(size_t) const;

    /// @brief Value setter funtion
    /// @param[in] index: The index to set to the value. (This is limit checked)
    /// @param[in] value: The value to load into the array
    /// @throws GCL::CCodeError
    /// @version 2015-08-14/GGB - Function created.

    template<typename T>
    void set(size_t index, T const &value)
    {
      RUNTIME_ASSERT(index < dim_, "Parameter index >= dim");

      switch(dataType)
      {
        case MCL::NT_int8:
        {
          reinterpret_cast<std::int8_t *>(data_)[index] = static_cast<std::int8_t>(value);
          break;
        };
        case MCL::NT_int16:
        {
          reinterpret_cast<std::int16_t *>(data_)[index] = static_cast<std::int16_t>(value);
          break;
        };
        case MCL::NT_int32:
        {
          reinterpret_cast<std::int32_t *>(data_)[index] = static_cast<std::int32_t>(value);
          break;
        };
        case MCL::NT_int64:
        {
          reinterpret_cast<std::int64_t *>(data_)[index] = static_cast<std::int64_t>(value);
          break;
        };
        case MCL::NT_uint8:
        {
          reinterpret_cast<std::uint8_t *>(data_)[index] = static_cast<std::uint8_t>(value);
          break;
        };
        case MCL::NT_uint16:
        {
          reinterpret_cast<std::uint16_t *>(data_)[index] = static_cast<std::uint16_t>(value);
          break;
        };
        case MCL::NT_uint32:
        {
          reinterpret_cast<std::uint32_t *>(data_)[index] = static_cast<std::uint32_t>(value);
          break;
        };
        case MCL::NT_uint64:
        {
          reinterpret_cast<std::uint64_t *>(data_)[index] = static_cast<std::uint64_t>(value);
          break;
        };
        case MCL::NT_float:
        {
          reinterpret_cast<float *>(data_)[index] = static_cast<float>(value);
          break;
        };
        case MCL::NT_double:
        {
          reinterpret_cast<double *>(data_)[index] = static_cast<double>(value);
          break;
        };
        default:
        {
          CODE_ERROR;
        };
      }
    }

    virtual void resize(size_t, bool = false);
    virtual void rebase(MCL::ENumericType);
    virtual MCL::ENumericType base() const { return dataType;}

    std::tuple<MCL::FP_t, MCL::FP_t> minmax() const;
    std::optional<MCL::FP_t> mean() const;
    std::optional<MCL::FP_t> stdev() const;
  };

  typedef std::shared_ptr<CArray1DP> PArray1DP;

}	// namespace SCL

#endif // SCL_ARRAY1DP_H
