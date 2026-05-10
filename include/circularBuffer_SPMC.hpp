//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                circularBuffer_lf.hpp
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2026 Gavin Blakeman.
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
// OVERVIEW:            Implements a lock free circular buffer designed to use with a Single Producer/Multiple Consumer paradigm.
//
// HISTORY:             2026-05-10/GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef SCL_CIRCULARBUFFERLF_HPP
#define SCL_CIRCULARBUFFERLF_HPP

  // Standard libraries

#include <array>
#include <atomic>
#include <concepts>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <optional>

// Miscellaneous libraries.
#include <GCL>

  // SCL Libraries
#include "common.h"
#include "config.h"

namespace SCL
{
  /*! @class    circular_buffer_SPMC
   *  @brief    Implements a fixed size circular buffer with a single producer multiple consumer model.
   *  @details  The Circular Buffer is a fixed size sequence container that allows elements to be pushed to the back of the
   *            container and elements to be popped from the front of the container. FIFO The Circular Buffer can also be referred
   *            to as  a Ring Buffer.
   *            The buffer is characterised by having a fixed size. Once the container is full, it can either drop any additional
   *             elements that are added, or overwrite the oldest element that is already in the buffer.
   *            The container is often used in a system where one thread is adding elements and another thread is removing elements.
   *            This is good for managing asynchronous operations.
   *            ***Items are pushed at the head, and popped from the tail of the buffer.***
   *  @note     1. A circularBuffer cannot be moved or copied.
   *  @note     2. The values in the buffer cannot be changed once pushed. They can be inspected or popped.
   *  @tparam   T: The type to be stored. Must be copy or move constructible.
   *  @tparam   bufferSize: The size of the buffer to allocate.
   *  @tparam   acceptDataWhenFull: true=> The buffer will overwrite the oldest data if full.
   */

  template<class T,
           std::size_t bufferSize = 1024,
           bool acceptDataWhenFull = false
           >
  requires std::move_constructible<T> || std::copy_constructible<T>
  class circular_buffer_SPMC
  {
  public:
    using value_type = T;
    using storage_type = std::array<value_type, bufferSize>;
    using size_type = typename storage_type::size_type;
    using pointer = value_type *;
    using const_pointer = value_type const *;
    using reference = value_type &;
    using const_reference = value_type const &;
    using mutex_type = std::mutex;

    static_assert(std::atomic<size_type>::is_always_lock_free, "Error: size_type atomics must be lock-free for the SPMC buffer.");


    /*! @brief      Constructor for the circular buffer class.
     *  @param[in]  extent: The extent of the buffer to create. IE how many elements can it hold.
     *  @throws     std::bad_alloc
     */
    explicit circular_buffer_SPMC() {}

    /*! @brief      Destructor for the circular buffer class.
     *  @details    Iterates the elements between head and tail and destroys them before deleting the storage.
     * @throws      None.
     */
    ~circular_buffer_SPMC() {}

    bool empty() const noexcept
    {
      return (elementCount == 0);
    }

    /*! @brief      Tests if the buffer is full.
     *  @returns    true if the buffer is full.
     */
    bool full() const noexcept
    {
      return elementCount == buffer.size();
    }

    /*! @brief      Returns the number of elements currently stored n the buffer.
     *  @returns    The number of elements in the buffer.
     *  @throws     noexcept.
     */
    size_type size() const noexcept
    {
      return elementCount;
    }

    /*! @brief      Returns the maximum size of the buffer. This is set at compile time and cannot be changed.
     *  @returns    The maximum number of elements that can be stored.
     */
    static constexpr size_type max_size() noexcept
    {
      return bufferSize;
    }

    /*! @brief      Returns the maximum size of the buffer. This is set at compile time and cannot be changed.
     *  @returns    The maximum number of elements that can be stored.
     */
    static constexpr size_type capacity() noexcept
    {
      return bufferSize;
    }

    /*! @brief Clears the circular buffer.
     *  @note 1. The buffer indexes are simply reset to zero.
     */
    void clear() noexcept
    {
      headIndex = tailIndex = 0;
      elementCount = 0;
    }

    /*! @brief      Pops the element from the tail of the buffer. This reduces the size by 1.
     * 	@param[out]	v: The buffer element std::moved into to v.
     * 	@returns 		true if a value was moved (or copied)
     *  @note       1. If the container is empty, the function does nothing.
     *  @throws     noexcept
     */
    bool pop(reference v) noexcept
    {
    	std::unique_lock<mutex_type> lock(consumerMutex);
      if (!empty())
      {
      	v = std::move(buffer[tailIndex++]);

      	tailIndex %= buffer.size();
        elementCount--;
        return true;
      }
      else
      {
      	return false;
      }
    }

    bool pop() noexcept
    {
    	std::unique_lock<mutex_type> lock(consumerMutex);
      if (!empty())
      {
      	tailIndex %= buffer.size();
        elementCount--;
        return true;
      }
      else
      {
      	return false;
      }
    }

    /*! @brief      Pushes a new value into the circular buffer.
     *  @param[in]  element: The element to add to the circular buffer.
     *  @throws     noexcept
     */
    bool push(value_type const &element) noexcept
    {
    	bool rv = false;
      if (elementCount >= buffer.max_size())
      {
        if constexpr (acceptDataWhenFull)
        {
          buffer[headIndex++] = element;
          tailIndex = headIndex + 1;
          rv = true;
        }
      }
      else
      {
        buffer[headIndex++] = element;
        elementCount++;
        rv = true;
      }
      if (headIndex >= buffer.size())
      {
        headIndex = headIndex % buffer.size();
      }

      return rv;
    }

    /*! @brief      Pushes a new value into the circular buffer.
     *  @param[in]  element: The element to add to the circular buffer.
     *  @throws     noexcept
     */
    bool push(value_type &&element) noexcept
    {
    	bool rv = false;
      if (size() == capacity())
      {
        if constexpr (acceptDataWhenFull)
        {
          buffer[headIndex++] = std::move(element);
          tailIndex = static_cast<size_type>(headIndex);
          rv = true;
        }
      }
      else
      {
        buffer[headIndex++] = std::move(element);
        elementCount++;
        rv = true;
      }
      if (headIndex >= buffer.size())
      {
        headIndex = headIndex % buffer.size();
      }
      return rv;
    }

  private:
    mutex_type consumerMutex;
    storage_type buffer;
    std::atomic<size_type> elementCount = 0;            ///< The number of elements contained.
    std::atomic<size_type> headIndex = 0;               ///< Index of the head (front) of the buffer.
    size_type tailIndex = 0;                						///< Index of the tail (rear) of the buffer.
    																										//   Unless == head, is pointing to a valid item.

    /* Prevent move and copy. */
    circular_buffer_SPMC(circular_buffer_SPMC const &) = delete;
    circular_buffer_SPMC(circular_buffer_SPMC &&) = delete;
    circular_buffer_SPMC &operator=(circular_buffer_SPMC const &) = delete;
    circular_buffer_SPMC &operator=(circular_buffer_SPMC &&) = delete;
  };
} // namespace SCL

#endif // SCL_CIRCULARBUFFERLF_HPP

