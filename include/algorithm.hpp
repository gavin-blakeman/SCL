//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Algorithm
// SUBSYSTEM:           Algorithms
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2019 Gavin Blakeman.
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
//                      2015-07-07 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef SCL_ALGORITHM_H
#define SCL_ALGORITHM_H

  // Standard libraries

#include <cstring>

  // Boost libraries

#include "boost/thread.hpp"

  // SCL Library

#include "common.h"
#include "config.h"

namespace SCL
{
  /// @brief Multi-threaded copy function.
  /// @param[in] src - Source to copy from.
  /// @param[in] dest - Destination to cop to.
  /// @param[in] len - Number of elements to copy.
  /// @throws None.
  /// @note This function will not work correctly if the memory areas are aliased.
  /// @version 2015-07-07/GGB - Function created.

  template<class T>
  void copy_mt(T *src, T *dest, size_t len)
  {
    size_t threadCount;
    size_t threadLen;
    boost::thread *thread;
    boost::thread_group threadGroup;
    size_t indexBegin = 0;

      // Ensure that we use a resonable number of threads.

    threadCount = len / 1000;

    if (threadCount == 0)
    {
      threadCount = 1;
      threadLen = len;
    }
    else if (threadCount > maxThreads)
    {
      threadCount = maxThreads;
      threadLen = len / threadCount;
    }
    else
    {
      threadLen = len / threadCount;
    };

      // Spawn the threads

    while (threadCount)
    {
      thread = new boost::thread( [](T *s, T *d, size_t l) { std::memcpy(d, s, l); }, &src[indexBegin], &dest[indexBegin], threadLen);
      threadGroup.add_thread(thread);
      thread = nullptr;

      indexBegin += threadLen;
      len -= threadLen;

      if (threadCount == 1)
      {
        threadLen = len;
      };
      threadCount--;
    };

    threadGroup.join_all();     // Wait for all the threads to finish.
  }
}

#endif // ALGORITHM_H
