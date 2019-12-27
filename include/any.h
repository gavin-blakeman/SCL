//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Any
// SUBSYSTEM:           Any
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2015-2019 Gavin Blakeman.
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
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-09-07 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef SCL_ANY_H
#define SCL_ANY_H

  // Standard C++ library header files.

#include <ostream>
#include <sstream>
#include <typeinfo>
#include <type_traits>

  // SCL header files

#include "Base.h"
#include "error.h"

namespace SCL
{
  /// The CAny Class allows any type of data to be stored. The class is a templatised class created on top of the CPackage class.

  class CAny
  {
    template<typename T>
    class TData : public CPackage
    {
    public:
      T *data = nullptr;

      TData() : data(nullptr) {}
      TData(T const &toCopy) : data(new T(toCopy)) {}
      virtual ~TData() { delete data; data = nullptr; }
      virtual CPackage *createCopy() const
      {
        TData *returnValue = new TData(*data);
        return dynamic_cast<CPackage *>(returnValue);
      }

      /// @brief Converts the value type to a string output. This is done by calling the insertion operator for the type.
      /// @returns The value as a std::string
      /// @throws
      /// @note 1. For 'char' types, the char is converted to a short before output. This ensure the value, not the char is
      ///          output.
      /// @version 2019-12-08/GGB - Updated to output char types as values.

      virtual std::string stringOutput() const
      {
        std::ostringstream valueStream;
        std::string returnValue;

        if constexpr(std::is_same<std::uint8_t, T>::value)
        {
          valueStream << static_cast<std::uint16_t>(*data);
        }
        else if constexpr(std::is_same<std::int8_t, T>::value)
        {
          valueStream << static_cast<std::int16_t>(*data);
        }
        else
        {
          valueStream << (*data);
        };

        returnValue = valueStream.str();
        return returnValue;
      }

      virtual std::type_info const &type() const noexcept { return typeid(T); }
    };


  private:
    CPackage *packageData = nullptr;

  protected:
  public:
    template<typename valueType>
    CAny(valueType const newData) : packageData(new TData<valueType>(newData)) {}
    CAny(CAny const &toCopy) : packageData(toCopy.packageData->createCopy()) {}

    virtual ~CAny() { delete packageData; packageData = nullptr; }

    virtual std::string stringOutput() const
    {
      if (packageData)
      {
        return packageData->stringOutput();
      }
      else
      {
        SCL_ERROR(0x0200);    // Null package.
      }
    }

    virtual std::type_info const &type() const noexcept { return packageData->type(); }

    friend std::ostream & operator<<(std::ostream &strm, CAny const &op) { strm << (op.packageData->stringOutput()); return strm; }

  };
}

#endif // SCL_ANY_H
