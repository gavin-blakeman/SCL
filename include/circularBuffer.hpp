//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Circular Buffer
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2017-2024 Gavin Blakeman.
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
// OVERVIEW:            1. Items are inserted into the binary tree in the correct sorted position
//                      2. The binary tree iterator will iterate the tree in the correct order. (Asc and Desc)
//
// HISTORY:             2017-07-21/GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef SCL_CIRCULARBUFFER_HPP
#define SCL_CIRCULARBUFFER_HPP

  // Standard libraries

#include <iterator>
#include <memory>
#include <mutex>
#include <shared_mutex>


  // SCL Libraries

#include "common.h"
#include "config.h"
  // Miscellaneous libraries.

#include <GCL>

namespace SCL
{
  /*! @class    circularBuffer
   *  @brief    Implements a fixed size circular buffer.
   *  @details  The Circular Buffer is a fixed size sequence container that allows elements to be pushed to the back of the
   *            container and elements to be popped from the front of the container. FIFO The Circular Buffer can also be referred
   *            to as  a Ring Buffer.
   *            The buffer is characterised by having a fixed size. Once the container is full, it can either drop any additional
   *             elements that are added, or overwrite the oldest element that is already in the buffer.
   *            The container is often used in a system where one thread is adding elements and another thread is removing elements.
   *            This is good for managing asynchronous operations.*
   *            Items are pushed at the back, and popped from the front of the buffer.
   *            The front of the buffer is at the tailPointer. The back of the buffer is at the head pointer.
   *  @note     1. A circularBuffer cannot be moved or copied.
   *  @note     2. The values in the buffer cannot be changed once pushed. They can be inspected or popped.
   *  @tparam   T: The type to be stored. Must be copy or move constructable.
   *  @tparam   bufferSize: The size of the buffer to allocate.
   *  @tparam   acceptDataWhenFull: true=> The buffer will overwrite the oldest data if full.
   *  @tparam   MTA: true=>Seperate threads for filling and emptying the buffer.
   */

  template<class T,
           std::size_t bufferSize = 1024,
           bool acceptDataWhenFull = true,
           bool MTA = false>
  class circularBuffer
  {
  public:
    using value_type = T;
    using storage_type = std::array<value_type, bufferSize>;
    using size_type = storage_type::size_type;
    using pointer = value_type *;
    using const_pointer = value_type const *;
    using reference = value_type &;
    using const_reference = value_type const &;


  private:
    using mutex_type = std::shared_mutex;

    struct empty_t{};
    mutable std::conditional_t<MTA, mutex_type, empty_t> mBuffer;

    template<class Mutex>
    struct FakeLockGuard{FakeLockGuard(Mutex&){}};

    using unique_lock = typename std::conditional_t<MTA, std::unique_lock<mutex_type>, FakeLockGuard<empty_t>>;
    using shared_lock = typename std::conditional_t<MTA, std::shared_lock<mutex_type>, FakeLockGuard<empty_t>>;

  public:

    /*! @brief      Constructor for the circular buffer class.
     *  @param[in]  extent: The extent of the buffer to create. IE how many elements can it hold.
     *  @throws     std::bad_alloc
     */
    explicit circularBuffer() {}

    /*! @brief Destructor for the circular buffer class.
     *  @details Iterates the elements between head and tail and destroys them before deleting the storage.
     * @throws None.
     */
    ~circularBuffer() {}

    /*! @brief      Returns a reference to the element at the given position in the circularBuffer. The position is relative to the
     *              tail pointer.
     *  @param[in]  index: Number of the element to get.
     *  @returns    Reference to the element
     *  @throws     std::out_of_range
     */
    const_reference operator[](size_type index) const
    {
      RUNTIME_ASSERT(index <= elementCount, "Requesting elements that does not exist.");

      index += tailIndex;
      if (index >= buffer.size())
      {
        index %= buffer.size();
      }

      return buffer[index];
    }

    bool empty() const noexcept
    {
      shared_lock sl(mBuffer);
      return (elementCount == 0);
    }

    /*! @brief      Tests if the buffer is full.
     *  @returns    true if the buffer is full.
     */
    bool full() const noexcept
    {
      shared_lock sl(mBuffer);
      return elementCount == buffer.size();
    }

    /*! @brief      Returns the number of elements currently stored n the buffer.
     *  @returns    The number of elements in the buffer.
     *  @throws     noexcept.
     */
    size_type size() const noexcept
    {
      shared_lock sl(mBuffer);
      return elementCount;
    }

    /*! @brief      Returns the maximum size of the buffer. This is set at compile time and cannot be changed.
     *  @returns    The maximum number of elements that can be stored.
     */
    size_type max_size() const noexcept
    {
      return buffer.size();
    }

    /*! @brief      Returns the maximum size of the buffer. This is set at compile time and cannot be changed.
     *  @returns    The maximum number of elements that can be stored.
     */
    size_type capacity() const noexcept
    {
      return buffer.size();
    }


    /*! @brief      Returns a const reference to the last element pushed into the circular buffer.
     *  @returns    const Reference to the last element.
     *  @throws     std::out_of_range
     */
    const_reference back() const
    {
      if (!empty())
      {
        shared_lock sl(mBuffer);

        std::size_t temp = headIndex - 1;
        temp %= buffer.size();

        return buffer[temp];
      }
      else
      {
        throw std::out_of_range("Circular Buffer empty, cannot access back() element.");
      }
    }

    /*! @brief Clears the circular buffer.
     *  @note 1. The buffer indexes are simply reset to zero.
     */
    void clear() noexcept
    {
      unique_lock ul{mBuffer};
      headIndex = tailIndex = 0;
      elementCount = 0;
    }

    /*! @brief Returns a reference to the first element in the circularBuffer.
     *  @returns A reference to the first element.
     *  @throws std::out_of_range
     */
    const_reference front() const
    {
      if (!empty())
      {
        shared_lock sl{mBuffer};

        return buffer[tailIndex];
      }
      else
      {
        throw std::out_of_range("Circular Buffer empty, cannot access front() element.");
      }
    }

    /*! @brief      Pops the element at the front of the buffer. This reduces the size of the container by 1.
     *  @note       1. If the container is empty, the function does nothing.
     *  @throws     noexcept
     */
    void pop() noexcept
    {
      if (!empty())
      {
        unique_lock ul{mBuffer};
        tailIndex++;
        tailIndex %= buffer.size();
        elementCount--;
      }
    }

    /*! @brief      Pushes a new value into the circular buffer.
     *  @param[in]  element: The element to add to the circular buffer.
     *  @throws     noexcept
     */
    void push(value_type const &element) noexcept
    {
      unique_lock ul(mBuffer);
      if (elementCount >= buffer.max_size())
      {


        if (acceptDataWhenFull)
        {
          buffer[headIndex++] = element;
          tailIndex = headIndex + 1;

        }
      }
      else
      {
        buffer[headIndex++] = element;
        elementCount++;
      }
      if (headIndex >= buffer.size())
      {
        headIndex %= buffer.size();
      }
    }

    /*! @brief      Pushes a new value into the circular buffer.
     *  @param[in]  element: The element to add to the circular buffer.
     *  @throws     noexcept
     */
    void push(value_type &&element) noexcept
    {
      if (size() == capacity())
      {
        unique_lock ul{mBuffer};
        if (acceptDataWhenFull)
        {
          buffer[headIndex++] = element;
          tailIndex = headIndex;
        }
      }
      else
      {
        unique_lock ul{mBuffer};

        buffer[headIndex++] = std::move(element);
        elementCount++;
      }
      if (headIndex >= buffer.size())
      {
        headIndex %= buffer.size();
      }
    }

  private:
    storage_type buffer;
    size_type elementCount = 0;             ///< The number of elements contained.
    size_type headIndex = 0;                ///< Index of the head (front) of the buffer.
    size_type tailIndex = 0;                ///< Index of the tail (rear) of the buffer.

    /* Prevent move and copy. */
    circularBuffer(circularBuffer const &) = delete;
    circularBuffer(circularBuffer &&) = delete;
    circularBuffer &operator=(circularBuffer const &) = delete;
    circularBuffer &operator=(circularBuffer &&) = delete;
  };
} // namespace SCL

#endif // SCL_CIRCULARBUFFER_HPP

