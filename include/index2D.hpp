//**********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								index2D
// SUBSYSTEM:						2D index class
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	None
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2020 Gavin Blakeman.
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
// OVERVIEW:            A 2-dimensional index class for use with vector2D and array 2D.
//
// CLASSES INCLUDED:    index2D - Two dimensional index.
//
// HISTORY:             2020-02-16 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef SCL_INDEX2D_HPP
#define SCL_INDEX2D_HPP

  // Standard C++ library header files.

#include <functional>
#include <type_traits>

namespace SCL
{
  template<typename T>
  class index2D
  {
    static_assert(std::is_integral<T>::value, "T must be unsigned integer type");
    static_assert(std::is_unsigned<T>::value, "T must be unsigned integer type");

    private:
    T row_;
    T column_;

  public:
    index2D() noexcept : row_(0), column_(0) {}
    index2D(T nRow, T nColumn) noexcept : row_(nRow), column_(nColumn) {}
    index2D(index2D const &other) noexcept : row_(other.row_), column_(other.column_) {}

    index2D operator=(index2D const &other) noexcept { row_(other.row_); column_(other.column_); }

    bool operator==(index2D const &other) const noexcept { return (row_ == other.row_) && (column_ == other.column_); }

    T row() const { return row_; }
    T &row() { return row_; }
    T column() const { return column_; }
    T &column() { return column_; }
  };
} // namespace SCL

  // Create the hash function for the index and inject it into the std namespace.

namespace std
{

  template<typename T> struct hash<SCL::index2D<T>>
  {
    /// @brief Hash function for the index2D type.
    /// @param[in] index: The index2D to compute the hash over.
    /// @returns a hashed value for the index.
    /// @throws None.
    /// @version 2020-02-16/GGB - Function created.

    std::size_t operator()(SCL::index2D<T> const &index) const noexcept
    {
      std::size_t h1 = std::hash<T>{}(index.row());
      std::size_t h2 = std::hash<T>{}(index.column());
      return h1 ^ (h2 << 1);
    }
  };
}

#endif // SCL_INDEX2D_HPP
