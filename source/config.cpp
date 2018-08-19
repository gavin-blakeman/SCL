//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library (SCL)
// FILE:								Config
// SUBSYSTEM:						Configuration file
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2018 Gavin Blakeman.
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
//
// CLASSES INCLUDED:		None
//
// CLASS HIERARCHY:     None.
//
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-07-02 GGB - File created.
//
//*********************************************************************************************************************************

#include "../include/config.h"

#include <thread>

namespace SCL
{
  size_t maxThreads = std::thread::hardware_concurrency();       // Maximum number of threads that an object will spawn.
}
