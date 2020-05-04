//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library (SCL)
// FILE:								Config
// SUBSYSTEM:						Configuration file
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2020 Gavin Blakeman.
//                      This file is part of the Storage Class Library (SCL)
//
//                      SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or
//                      (at your option) any later version.
//
//                      SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with SCL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
//
// OVERVIEW:						Configuration values and typedefs for the library.
//
// CLASSES INCLUDED:		None
//
// CLASS HIERARCHY:     None.
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-07-02 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef SCL_CONFIG_H
#define SCL_CONFIG_H

  // Standard library

#include <cstddef>
#include <cstdint>

/// @file
/// The config.h file is used for storing configuration parameters and macros for the Storage Class Library. The parameters
/// should be used throughout the library. Changing the typedefs in this file and recompiling will allow the library to use
/// different sizes for various types.

namespace SCL
{
  extern size_t maxThreads;                   ///< The maximun number of threads to use in multi-threaded functions.
                                              ///< The library will choose a number of threads to utilise up to a maximum of this
                                              ///< value. This is a run-time value that can be changed by the controlling
                                              /// application. The default value is 1.

#ifndef SCL_NOMT
# define SCL_THREAD
#endif

#define USE_GCL   /* Define this to use the GCL error library. If not defined, the code will simply use std::exception. */

}    // namespace SCL

#endif // SCL_CONFIG_H
