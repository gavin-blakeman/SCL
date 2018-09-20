//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Circular Buffer
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018 Gavin Blakeman.
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

  // SCL Libraries

#include "common.h"
#include "config.h"

  // Standard libraries

#include <iterator>
#include <memory>
#include <mutex>

  // Miscellaneous libraries.

#include <GCL>

namespace SCL
{

  template<typename T,
           typename T_nonconst,
           typename elem_type = typename T::value_type>
  class TCircularBufferIterator
  {
  public:
    typedef TCircularBufferIterator<T, T_nonconst, elem_type>   self_type;
    typedef T                                                   CBType;
    typedef std::random_access_iterator_tag                     iterator_category;
    typedef typename CBType::value_type                         value_type;
    typedef typename CBType::size_type                          size_type;
    typedef typename CBType::pointer                            pointer;
    typedef typename CBType::const_pointer                      const_pointer;
    typedef typename CBType::reference                          reference;
    typedef typename CBType::const_reference                    const_reference;
    typedef typename CBType::difference_type                    difference_type;


  private:
    CBType *circularBuffer;
    size_t index_;

  protected:
  public:
    friend class TCircularBufferIterator<const T, T, const elem_type>;

    TCircularBufferIterator(CBType *bt, size_t n) : circularBuffer(bt), index_(n) {}
    TCircularBufferIterator(TCircularBufferIterator<T_nonconst, T_nonconst, typename T_nonconst::value_type> const &other)
      : circularBuffer(other.circularBuffer), index_(other.index_) {}

    elem_type &operator*() { return (*circularBuffer)[index_]; }
    elem_type *operator->() { return *(operator*()); }

    self_type &operator++()
    {
      index_ += 1;
      return *this;
    }

    self_type operator++(int)
    {
      self_type tmp(*this);
      ++(*this);
      return tmp;
    }

    self_type &operator--()
    {
      index_ -= 1;
      return *this;
    }

    self_type operator--(int)
    {
      self_type tmp(*this);
      --(*this);
      return tmp;
    }

    self_type operator+(difference_type n) const
    {
      self_type tmp(*this);
      tmp.index_ += n;
      return tmp;
    }

    self_type operator+=(difference_type n)
    {
      index_ += n;
      return *this;
    }

    self_type operator-(difference_type n) const
    {
      self_type tmp(*this);
      tmp.index_ += n;
      return tmp;
    }

    self_type operator-=(difference_type n)
    {
      index_ += n;
      return *this;
    }

    difference_type operator-(const self_type &c) const
    {
      return index_ - c.index_;
    }

    bool operator==(const self_type &other) const
    {
      return index_ == other.index_ && circularBuffer == other.circularBuffer;
    }
    bool operator!=(const self_type &other) const
    {
      return index_ != other.index_ && circularBuffer == other.circularBuffer;
    }
    bool operator>(const self_type &other) const
    {
      return index_ > other.index_;
    }
    bool operator>=(const self_type &other) const
    {
      return index_ >= other.index_;
    }
    bool operator<(const self_type &other) const
    {
      return index_ < other.index_;
    }
    bool operator<=(const self_type &other) const
    {
      return index_ <= other.index_;
    }
  };

  template <typename circularBufferIterator_t>
  circularBufferIterator_t operator+ (const typename circularBufferIterator_t::difference_type &a,
                                      const circularBufferIterator_t                           &b)
  {
    return circularBufferIterator_t(a) + b;
  }

  template <typename circularBufferIterator_t>
  circularBufferIterator_t operator- (const typename circularBufferIterator_t::difference_type &a,
                                      const circularBufferIterator_t                           &b)
  {
    return circularBufferIterator_t(a) - b;
  }

  /// @class TCircularBuffer
  /// @brief Implements a fixed size circular buffer.
  /// @details The Circular Buffer is a fixed size sequence container that allows elements to be pushed to the back of the container
  ///          and elements to be popped from the front of the container. The Circular Buffer can also be referred to as a Ring
  ///          Buffer.
  ///          The buffer is characterised by having a fixed size. Once the container is full, it can either drop any additional
  ///          elements that are added, or overwrite the oldest element that is already in the buffer.
  ///          The container is often used in a system where one thread is adding elements and another thread is removing elements.
  ///          This is good for managing asynchronous operations.
  /// @todo 1. Needs to be debugged.

  template<class T,
           bool acceptDataWhenFull = true,
           class Alloc_ = std::allocator<T>>
  class TCircularBuffer
  {
  public:
    typedef TCircularBuffer<T, acceptDataWhenFull, Alloc_>                          self_type;
    typedef Alloc_                                                                  allocator_type;
    typedef typename Alloc_::value_type                                             value_type;
    typedef typename Alloc_::pointer                                                pointer;
    typedef typename Alloc_::const_pointer                                          const_pointer;
    typedef typename Alloc_::reference                                              reference;
    typedef typename Alloc_::const_reference                                        const_reference;
    typedef typename Alloc_::difference_type                                        difference_type;
    typedef typename Alloc_::size_type                                              size_type;
    typedef TCircularBufferIterator<self_type, self_type>                           iterator;
    typedef TCircularBufferIterator<const self_type, self_type, const value_type>   const_iterator;
    typedef std::reverse_iterator<iterator>                                         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                                   const_reverse_iterator;

  private:
    allocator_type alloc_;
    pointer bufferArray;                    ///< Memory array for the buffer contents.
    size_type bufferExtent;                 ///< The extent of the circular buffer. IE how many elements can it hold.
    size_type elementCount;                 ///< The number of elements contained.
    size_type headIndex;                    ///< Index of the head (front) of the buffer.
    size_type tailIndex;                    ///< Index of the tail (rear) of the buffer.

#ifdef SCL_THREAD
    std::mutex classMutex_;
#endif

    size_type normalise(size_type n) const noexcept { return n % bufferExtent; }
    size_type indexToSubscript(size_type index) const noexcept { return normalise(index + headIndex); }

    template<typename f_iter>
    void assignInto(f_iter from, f_iter to)
    {
      if (size()) clear();
      while (from != to)
      {
        push_back(*from);;
        ++from;
      };
    }

    template<typename f_iter>
    void assignIntoReserving(f_iter from, f_iter to)
    {
      if (elementCount) clear();
      while (from != to)
      {
        if (elementCount == bufferExtent)
        {
          reserve(static_cast<size_type>(bufferExtent * 1.5));
        }
        push_back(*from);
        ++from;
      }
    }

    /// @brief Destroys all allocated elements.
    /// @throws None.
    /// @version 2017-07-21/GGB - Function created.

    void destroyAllElements()
    {
      for (size_type n = 0; n < elementCount; ++n)
      {
        alloc_.destroy(bufferArray[indexToSubscript(n)]);
      }
    }

  protected:

    /// @brief Increments the tail index.
    /// @throws None.
    /// @version 2017-07-21/GGB - Function created.

    void incrementTail() noexcept
    {
      ++tailIndex;
      if (tailIndex == bufferExtent)
      {
        tailIndex = 0;
      };
      ++elementCount;
    }

    /// @brief Increment the head index.
    /// @throws None.
    /// @version 2017-07-21/GGB - Function created.

    void incrementHead() noexcept
    {
      if (!empty())
      {
        ++headIndex;
        --elementCount;
        if (headIndex == bufferExtent)
        {
          headIndex = 0;
        };
      };
    }

  public:

    /// @brief Constructor for the circular buffer class.
    /// @param[in] extent - The extent of the buffer to create. IE how many elements can it hold.
    /// @throws std::bad_alloc
    /// @version 2017-07-18/GGB - Function created.

    explicit TCircularBuffer(size_type extent = 1024) : bufferArray(alloc_.allocate(bufferExtent)), bufferExtent(extent),
      elementCount(0), headIndex(0), tailIndex(0), classMutex_() {}

    TCircularBuffer(TCircularBuffer const &other) : bufferArray(alloc_.allocate(other.bufferExtent)),
      bufferExtent(other.bufferExtent), elementCount(other.elementCount), headIndex(other.headIndex), tailIndex(other.tailIndex),
      classMutex_()
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      try
      {
        assignInto(other.begin(), other.end());
      }
      catch(...)
      {
        destroyAllElements();
        alloc_.deallocate(bufferArray, bufferExtent);
        throw;
      }
    }

    template <class InputIterator>
    TCircularBuffer(InputIterator from, InputIterator to) : bufferArray(alloc_.allocate(1)), bufferExtent(1), elementCount(0),
      headIndex(0), tailIndex(0), classMutex_()
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      TCircularBuffer tmp;
      tmp.assignIntoReserving(from, to);
      swap(tmp);
    }

    /// @brief Destructor for the circular buffer class.
    /// @details Iterates the elements between head and tail and destroys them before deleting the storage.
    /// @throws None.
    /// @version 2017-07-19/GGB - Function created.

    ~TCircularBuffer()
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      destroyAllElements();
      alloc_.deallocate(bufferArray, bufferExtent);
    }

    TCircularBuffer &operator=(const self_type &other)
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      TCircularBuffer tmp(other);
      swap(tmp);
      return *this;
    }

    /// @brief Swaps the contents of two circular buffers.
    /// @param[in] other - The other buffer to swap.
    /// @throws None.
    /// @version 2017-07-21/GGB - Function created.

    void swap(TCircularBuffer &other) noexcept
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      std::swap(bufferArray,      other.bufferArray);
      std::swap(bufferExtent,     other.bufferExtent);
      std::swap(headIndex,        other.headIndex);
      std::swap(tailIndex,        other.tailIndex);
      std::swap(elementCount,     other.elementCount);
    }

    /// @brief Returns a reference to the element at the given position in the circularBuffer.
    /// @param[in] n - Number of the element to get. Note this is referenced to the headIndex, not the start of the buffer.
    /// @returns Reference to the element
    /// @throws std::out_of_range
    /// @version 2017-07-20/GGB - Function created.

    const_reference operator[](size_type index) const
    {
      return at(index);
    }

    /// @brief Returns a reference to the element at the given position in the circularBuffer.
    /// @param[in] n - Number of the element to get. Note this is referenced to the headIndex, not the start of the buffer.
    /// @returns Reference to the element
    /// @throws std::out_of_range
    /// @version 2017-07-21/GGB - Function created.

    reference at(size_type index) const
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      if (index < elementCount)
      {
        return bufferArray[indexToSubscript(index)];
      }
      else
      {
        throw std::out_of_range("Attempt to access element outside of circular buffer.");
      }
    }

    iterator begin() { std::lock_guard<std::mutex> lg(classMutex_); return iterator(this, 0); }
    iterator end()   { std::lock_guard<std::mutex> lg(classMutex_); return iterator(this, size()); }

    const_iterator begin() const { std::lock_guard<std::mutex> lg(classMutex_); return const_iterator(this, 0); }
    const_iterator end() const   { std::lock_guard<std::mutex> lg(classMutex_); return const_iterator(this, size()); }

    reverse_iterator rbegin() { std::lock_guard<std::mutex> lg(classMutex_); return reverse_iterator(end()); }
    reverse_iterator rend() { std::lock_guard<std::mutex> lg(classMutex_); return reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const { std::lock_guard<std::mutex> lg(classMutex_); return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { std::lock_guard<std::mutex> lg(classMutex_); return const_reverse_iterator(begin()); }

    bool empty() const noexcept { std::lock_guard<std::mutex> lg(classMutex_); return (elementCount == 0); }
    size_type size() const noexcept { std::lock_guard<std::mutex> lg(classMutex_); return elementCount; }
    size_type max_size() const noexcept { return alloc_.max_size(); }
    size_type capacity() const noexcept { std::lock_guard<std::mutex> lg(classMutex_); return bufferExtent; }

    /// @brief Returns a reference to the last element pushed into the circular buffer.
    /// @returns Reference to the last element.
    /// @throws std::out_of_range
    /// @version 2017-07-19/GGB - Function created.

    reference back()
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      if (!empty())
      {
        return bufferArray[tailIndex];
      }
      else
      {
        throw std::out_of_range("Circular Buffer empty, cannot access back() element.");
      }
    }

    /// @brief Returns a const reference to the last element pushed into the circular buffer.
    /// @returns const Reference to the last element.
    /// @throws std::out_of_range
    /// @version 2017-07-19/GGB - Function created.

    const_reference back() const
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      if (!empty())
      {
        return bufferArray[tailIndex];
      }
      else
      {
        throw std::out_of_range("Circular Buffer empty, cannot access back() element.");
      }
    }


    /// @brief Clears the circular buffer.
    /// @param[in] size- The size of the buffer to create.
    /// @throws std::bad_alloc
    /// @note 1. The buffer indexes are simply reset to zero and the data is deleted.
    /// @version 2017-07-18/GGB - Function created.

    void clear() noexcept
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      destroyAllElements();
      headIndex = tailIndex = 0;
      elementCount = 0;
    }

    /// @brief Returns a reference to the first element in the circularBuffer.
    /// @returns A reference to the first element.
    /// @throws std::out_of_range
    /// @version 2017-07-19/GGB - Function created.

    reference front()
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      if (!empty())
      {
        return bufferArray[headIndex];
      }
      else
      {
        throw std::out_of_range("Circular Buffer empty, cannot access front() element.");
      }
    }

    /// @brief Returns a reference to the first element in the circularBuffer.
    /// @returns A reference to the first element.
    /// @throws std::out_of_range
    /// @version 2017-07-19/GGB - Function created.

    const_reference front() const
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      if (!empty())
      {
        return bufferArray[headIndex];
      }
      else
      {
        throw std::out_of_range("Circular Buffer empty, cannot access front() element.");
      }
    }

    /// @brief Returns a copy of the allocator object associated with the circular buffer.
    /// @returns The allocator.
    /// @throws None.
    /// @version 2017-07-19/GGB - Function created.

    allocator_type get_allocator() const noexcept { return alloc_; }

    /// @brief Pops the element at the front of the buffer. This reduces the size of the container by 1.
    /// @note 1. If the container is empty, the function does nothing.
    /// @throws None.
    /// @version 2017-07-19/GGB - Function created.

    void pop_front()
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      if (!empty())
      {
        bufferArray.pop_back();
      }
    }

    /// @brief Pushes a new value into the circular buffer.
    /// @param[in] element - Thje element to add to the circular buffer.
    /// @throws None.
    /// @version 2017-07-19/GGB - Function created.

    void push_back(value_type const &element) noexcept
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      size_type next = (tailIndex+1 == capacity()) ? 0 : tailIndex+1;
      if (size() == capacity())
      {
        if (acceptDataWhenFull)
        {
          bufferArray[next] = element;
          incrementHead();
        }
      }
      else
      {
        alloc_.construct(bufferArray[next], element);
      }
      incrementTail();
    }

    /// @brief Pushes a new value into the circular buffer.
    /// @param[in] element - Thje element to add to the circular buffer.
    /// @throws None.
    /// @version 2017-07-19/GGB - Function created.

    void push_back(value_type &&element) noexcept
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      size_type next = (tailIndex+1 == capacity()) ? 0 : tailIndex+1;
      if (size() == capacity())
      {
        if (acceptDataWhenFull)
        {
          bufferArray[next] = element;
          incrementHead();
        }
      }
      else
      {
        alloc_.construct(bufferArray[next], element);
      }
      incrementTail();
    }

    void reserve(size_type newSize)
    {
      std::lock_guard<std::mutex> lg(classMutex_);
      if (capacity() < newSize)
      {
        TCircularBuffer tmp(newSize);
        tmp.assignInto(begin(), end());
        swap(tmp);
      }
    }
  };

  template <typename T,
            bool consumePolicy,
            typename Alloc>
  bool operator==(const TCircularBuffer<T, consumePolicy, Alloc> &a,
                  const TCircularBuffer<T, consumePolicy, Alloc> &b)
  {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
  }

  template <typename T,
            bool consumePolicy,
            typename Alloc>
  bool operator!=(const TCircularBuffer<T, consumePolicy, Alloc> &a,
                  const TCircularBuffer<T, consumePolicy, Alloc> &b)
  {
    return a.size() != b.size() || !std::equal(a.begin(), a.end(), b.begin());
  }

  template <typename T,
            bool consumePolicy,
            typename Alloc>
  bool operator<(const TCircularBuffer<T, consumePolicy, Alloc> &a,
                 const TCircularBuffer<T, consumePolicy, Alloc> &b)
  {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
  }

} // namespace SCL

#endif // SCL_CIRCULARBUFFER_HPP

