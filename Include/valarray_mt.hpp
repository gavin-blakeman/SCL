//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                valarray_mt.h
// SUBSYSTEM:           Multi-threaded valarray extension
// LANGUAGE:						C++
// TARGET OS:           None
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						libAstometry
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2014, 2017 Gavin Blakeman.
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
// OVERVIEW:            This is an inherited class from std::valarray that extends valarray functionality to allow multi-threading
//                      of the worker functions. This is done by overloading functions as required.
//
// CLASSES INCLUDED:    SCL::valarray_mt
//
// CLASS HIERARCHY:     std::valarray
//                        SCL::valarray_mt
//
// NOTE:                This class is complete and usable. However, all overloads of the std::valarray functions have not been
//                      written.
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-01-14 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef SCL_VALARRAY_MT_HPP
#define SCL_VALARRAY_MT_HPP

#include <valarray>

namespace SCL
{
  template<typename T>
  class valarray_mt : public std::valarray
  {
  private:
    void sumThread(size_t, size_t, T) const;

  protected:
  public:
    T sum() const;
  };

  /// @brief Thread function called by SCL::valarray_mt::sum(...) to determine the sum of an array
  /// @throws None.
  //
  // 2014-01-13/GGB - Function created.

  template<typename T>
  void valarray_mt::sumThread(size_t indexStart, size_t indexEnd, T &sum)
  {
    size_t index;
    sum = 0;

    for(index = indexStart; index < indexEnd; index++)
    {
      sum += (*this)[index];
    }
  }

  /// @brief Multi-threaded sum function for valarray_mt.
  //
  // 2014-01-14/GGB - Function created.

  template<typename T>
  T valarray_mt::sum() const
  {
    size_t numberOfThreads;
    size_t threadNumber;
    size_t stepSize;
    boost::thread_group threadGroup;
    boost::thread *thread;
    size_t indexBegin = 0, indexEnd = 0;
    size_t index;
    T returnValue = 0;
    size_t count = data.size();

    if (count != 0)
    {
       // Ensure that we are using a reasonable number of threads. Maximise the number of threads to the number of values

      numberOfThreads = count / 1000;
      if (numberOfThreads == 0)
        numberOfThreads = 1;
      else if (numberOfThreads > maxThreads)
        numberOfThreads = maxThreads;

      stepSize = count / numberOfThreads;

      boost::scoped_array<FP_t> sums(new T[numberOfThreads]);

        // Spawn the threads.

      for (threadNumber = 0; threadNumber < numberOfThreads; threadNumber++)
      {
        indexBegin = indexEnd;
        if (threadNumber == (numberOfThreads - 1) )
          indexEnd = count;
        else
          indexEnd += stepSize;
        thread = new boost::thread(&sumThread<T>, boost::cref(*this), indexBegin, indexEnd, boost::ref(sums[threadNumber]));
        threadGroup.add_thread(thread);
        thread = nullptr;
      };

      threadGroup.join_all();     // Wait for all the threads to finish.

      for(index = 0; index < numberOfThreads; index++)
        returnValue += sums[index];

      return returnValue;
  }


} // namespace

#endif // SCL_VALARRAY_MT_HPP
