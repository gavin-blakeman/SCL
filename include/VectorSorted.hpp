//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								VectorSorted
// SUBSYSTEM:						Sorted Vector implementation
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2012-2020 Gavin Blakeman.
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
// OVERVIEW:						This is a templated sorted vector implementation.
//
// CLASSES INCLUDED:		CVectorSorted            - A sorted vector implementation
//
// CLASS HIERARCHY:     CContainer
//                        CSortedContainer
//                          TVectorSorted<T>
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-12-31 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef SCL_VECTORSORTED_H
#define SCL_VECTORSORTED_H

#include "Base.h"

namespace SCL
{
  template<class T>
  class TVectorSorted : public CSortedContainer
  {
  public:
    typedef CSortedContainer::size_type size_type;

    class iterator
    {
    private:
      size_type index;
      TVectorSorted *vector;
    protected:
      iterator(TVectorSorted *v, size_type i) : index(i), vector(v) {}

    public:
      iterator() : vector(nullptr) {}

      iterator operator++()
      {
        if (vector)
        {
          if (index != vector->size())
            index++;
        };
        return *this;
      }

      iterator operator--()
      {
        if (vector)
        {
          if (index != 0)
            index--;
        };
        return *this;
      }

      iterator operator++(int)
      {
        if (vector)
        {
          if (index != vector->size())
            index++;
        };
        return *this;
      }

      iterator operator--(int)
      {
        if (vector)
        {
          if (index != 0)
            index--;
        };
        return *this;
      }

      bool operator==(iterator const &rhs) const
      {
        return ( (vector == rhs.vector) && (index == rhs.index) );
      }

      bool operator!=(iterator const &rhs) const
      {
        return !(*this == rhs);
      }

      T operator*()
      {
        return (*vector)[index];
      }

      iterator &operator=(iterator const &rhs)
      {
        vector = rhs.vector;
        index = rhs.index;

        return *this;
      }

      friend class TVectorSorted;
    };

  private:
    T *data;
    size_type resizeSize_;
    size_type capacity_;

  protected:
    void internalResize();

  public:
    TVectorSorted(size_type initialsize = 1000);
    virtual ~TVectorSorted();

    virtual T &operator[](size_type);

    virtual void reserve(size_type);
    virtual void resize(size_type);
    virtual void insert(T);
    virtual void clear();

    /// Erases a single element as specified by the iterator from the container.
    /// EXCEPTIONS: 0x2000 - VectorSorted: Incorrect iterator for object.
    /// @version 2017-06-25/GGB - Updated to use new GCL error handling.
    // 2013-01-01/GGB - Function created.

    iterator erase(iterator toErase)
    {
      size_type index;

      if (toErase.vector != this)
      {
        SCL_ERROR(0x2000);    // VectorSorted: Incorrect iterator for object.
      }
      else
      {
        index = toErase.index;
        data[index] = T();                // Erase the object/
        for (; index < size() - 1; index++)   // Move the rest of the items down in the array
          data[index] = data[index+1];
        data[index] = T();
        decPackageCount();
        return toErase;
      };
    }

    /// @brief Erases a range of elements.
    /// @throws 0x2000 - VectorSorted: Incorrect iterator for object.
    /// @version 2017-06-25/GGB - Updated to use new GCL error handling.
    /// @version 2013-04-21/GGB - When erasing to end of array elements are not null constructed. Just left as they where.
    /// @version 2013-01-01/GGB - Function created.

    virtual iterator erase(iterator eraseBegin, iterator eraseEnd)
    {
      size_type index1, index2, index3, indexBegin;
      size_type span;

      if ( (eraseBegin.vector != this) || (eraseEnd.vector != this) )
      {
        SCL_ERROR(0x2000);    // VectorSorted: Incorrect iterator for object.
      }
      else
      {
        indexBegin = eraseBegin.index;      // Store this value for return

        if (eraseEnd == end())      // Erasing to the end of the array. Just null construct all the elements.
        {
          //for (index1 = eraseBegin.index; index1 < size(); index1++)
          //  data[index1] = T();
          setPackageCount(indexBegin + 1);
        }
        else
        {
          span = eraseEnd.index - eraseBegin.index;

          if ( (eraseEnd.index + span) > size() )    // Erase more than number of elements to be copied.
          {
            span = size() - eraseEnd.index;

            index2 = eraseBegin.index;
            index3 = eraseEnd.index;
            for (index1 = 0; index1 < span; index1++)
              data[index2++] = data[index3++];

            //for(; index2 < size(); index2++)
            //  data[index2] = T();                 // null construct to end of array
            setPackageCount(indexBegin+1);
          }
          else      // Erasing within vector only
          {
            index3 = eraseEnd.index;
            for (index2 = eraseBegin.index; index2 < size(); index2++)
              data[index2] = data[index3++];

            //for (;index3 < size(); index3++)
            //  data[index3] = T();                 // null construct to end of array
            setPackageCount(size() - (eraseEnd.index - eraseBegin.index));
          };
        };
        return iterator(this, indexBegin + 1);
      };
    }

    /// @brief Finds the element = val. If no element is found, then the end() is returned.
    /// @throws
    /// @version 2013-01-01/GGB - Function created.

    virtual iterator find(T const &val)
    {
      size_type s = size();
      size_type index, lastIndex, priorIndex;
      bool found = false;

      lastIndex = 0;
      index =  s / 2;

      if (empty())
      {
        return end();     // Trivial case, return the last element
      }
      else
      {
        while ( (!found) && (index != lastIndex) )
        {
          if (val == data[index])
            found = true;
          else if ( val  < data[index] )
          {   // Need to be moving left
            priorIndex = index;
            if (index > lastIndex)
              index += (s - index) / 2;
            else
              index += (lastIndex - index) / 2;
            lastIndex = priorIndex;
          }
          else
          { // Too far to the right of the insertion point.
            priorIndex = index;
            if (index < lastIndex)
              index -= index / 2;
            else
              index -= (index - lastIndex) / 2;
            lastIndex = priorIndex;
          };
        };
        if (found)
          return iterator(this, index);
        else
          return end();   // Not found.
      };
    }

    /// Creates an iterator pointing to the beginning.
    //
    // 2012-12-31/GGB - Function created.

    virtual inline iterator begin()
    {
      iterator returnValue(this, 0);
      return returnValue;
    }

    /// Creates an iterator pointing to the end.
    //
    // 2012-12-31/GGB - Function created

    virtual iterator end()
    {
      iterator returnValue(this, size());
      return returnValue;
    }

  };

  // Constructor for the class.
  //
  // 2012-12-31/GGB - Function created.

  template<class T>
  inline TVectorSorted<T>::TVectorSorted(size_type initialsize) : CSortedContainer(), data(nullptr), resizeSize_(initialsize),
    capacity_(initialsize)
  {
    data = new T[initialsize];
  }

  /// Destructor for the class.
  //
  // 2012-12-31/GGB - Function created.

  template<class T>
  inline TVectorSorted<T>::~TVectorSorted()
  {
    delete [] data;
    data = nullptr;
    resetPackageCount();
  }

  /// Non-const operator[] for the class.
  /// EXCEPTIONS: 0x2001 - VectorSorted: Index out of bounds [0; size()].
  /// @version 2017-06-25/GGB - Updated to use new GCL error handling.
  // 2013-01-01/GGB - Function created.

  template<class T>
  inline T &TVectorSorted<T>::operator[] (size_type index)
  {
    if ( (index >= size()) )
    {
      SCL_ERROR(0x2001);      // VectorSorted: Index out of bounds [0; size()].
    }
    else
    {
      return data[index];
    }
  }

  /// Clears all the items from the list.
  //
  // 2013-01-02/GGB - Function created.

  template<class T>
  inline void TVectorSorted<T>::clear()
  {
    size_type index = 0;

    while (index < size())
    {
      data[index++] = T();
    }

    resetPackageCount();
  }

  /// Inserts a new object into the list. The sort order is always maintained using the package < operator.
  //
  // 2013-04-25/GGB - Added code to allow equal value packages to be stored.
  // 2012-12-31/GGB - Function created.

  template<class T>
  void TVectorSorted<T>::insert(T p)
  {
    size_type index, highIndex, lowIndex, index2;

    if (size() >= capacity_)
    {
      reserve(capacity_ + resizeSize_);
      resizeSize_ = capacity_;               // Causes a doubling of the package each time.
    };

    lowIndex = 0;
    highIndex = size();

    while ( lowIndex < highIndex )
    {
      index = lowIndex + (highIndex - lowIndex) / 2;
      if ( p < data[index] )
        highIndex = index ;
      else if (data[index] < p)
        lowIndex = index + 1;
      else                                    // GGB - Added to allow identical packages to be stored.
        lowIndex = highIndex = index;         // GGB - Added to allow identical packages to be stored.
    };

    for (index2  = size(); index2 > lowIndex; index2--)
      data[index2] = data[index2-1];

    data[lowIndex] = p;
    incPackageCount();
  };

  /// Increases the storage size if required.
  //
  // 2012-12-31/GGB - Function created.

  template<class T>
  void TVectorSorted<T>::reserve(size_type r)
  {
    T *newData;
    size_type index;

    if (r > capacity_)
    {
      newData = new T[r];
      for (index = 0; index < size(); index++)
        newData[index] = data[index];
      delete [] data;
      data = newData;
      newData = nullptr;
      capacity_ = r;
      resizeSize_ = r;
    };
  }

  /// Resizes the vector. If the size is larger, then the packages are copied over. If the size is less
  /// that the current size, then the excess packages are discarded (and destroyed)
  //
  // 2012-12-31/GGB - Function created.

  template<class T>
  void TVectorSorted<T>::resize(size_type r)
  {
    T *newData;
    size_type index;

    if (r > capacity_)
    {
      reserve(r);               // Reserve function does what is required here
    }
    else if (r < capacity_)
    {
      newData = new T[r];
      for (index = 0; index < r; index++)
        newData[index] = data[index];

      delete [] data;
      data = newData;
      newData = nullptr;
      capacity_ = r;
      resizeSize_ = r;
      setPackageCount(r);
    };
  }

}    // namespace SCL

#endif  // SCL_VECTORSORTED_H
