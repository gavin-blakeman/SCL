//**********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Base.h
// SUBSYSTEM:           Base Class for Storage Class Library
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2010, 2013-2020 Gavin Blakeman.
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
// OVERVIEW:            Provides a group of storage class implementations.
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2015-05-21 GGB - Added functions to CPackage to support CAny.
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2010-04-28 GGB - Unscrabble Application Release 1.00
//
//**********************************************************************************************************************************

#ifndef SCL_BASE_H
#define SCL_BASE_H

  // Standard C++ library header files.

#include <memory>
#include <ostream>
#include <typeinfo>

  // Miscellaneous library header files

#include <GCL>

  // SCL header files.

#include "common.h"

namespace SCL
{
  /// @brief A base base class for other classes to utilise.
  /// @note 1. This is used by the CAny class.

  class CPackage
  {
  private:
  protected:
  public:
    CPackage() {}
    virtual  ~CPackage() {}
    virtual CPackage *createCopy()  { return nullptr; }

    inline virtual bool operator==(const CPackage &toTest) { return this == &toTest; }	// Check pointers equal.

    virtual std::string stringOutput() const { return std::string(); }

    virtual std::type_info const &type() const noexcept { return typeid(CPackage); }
  };
  [[deprecated]] typedef CPackage *PPackage;

  class [[deprecated]] CSortablePackage : public CPackage
  {
  private:
  protected:
  public:
    CSortablePackage() {}
    virtual ~CSortablePackage() {}

    virtual bool operator<(CSortablePackage const &) const { CODE_ERROR(); }
    virtual bool operator==(CSortablePackage const &) const { CODE_ERROR(); }
  };

  [[deprecated]] typedef CSortablePackage *PSortablePackage;

  class [[deprecated]] CContainer
  {
  public:
    typedef unsigned long size_type;
  private:
    size_type lPackageCount;						// Number of packages in the container
    bool bOwnsPackages;									// True if the container owns the contained packages

  protected:
    inline void incPackageCount() {lPackageCount++;}
    inline void decPackageCount() {lPackageCount--;}
    inline void resetPackageCount() {lPackageCount = 0;}
    inline void setPackageCount(size_type n) { lPackageCount = n; }

  public:
    CContainer() : lPackageCount(0), bOwnsPackages(true) {}
    virtual ~CContainer() {}

    virtual bool insert(CPackage *) { CODE_ERROR(); }  // Insert an object into the container.
    virtual bool erase(CPackage *) { CODE_ERROR(); }
    virtual void clear() { CODE_ERROR(); }

    inline size_type size() const {return lPackageCount;}

    inline bool ownsPackages() const { return bOwnsPackages; }
    inline void ownsPackages(bool newOwner) { bOwnsPackages = newOwner; }

    inline bool empty() const { return lPackageCount == 0; }
  };

  class [[deprecated]] CUnsortedContainer : public CContainer
  {
  private:
  protected:
  public:
    CUnsortedContainer() {}
    virtual ~CUnsortedContainer() {}
  };

  class [[deprecated]]CSortedContainer : public CContainer
  {
  private:
  protected:
  public:
    CSortedContainer() : CContainer() {}
    virtual ~CSortedContainer() {}

  };

  class [[deprecated]] CIterator
  {
  private:
    bool bEOC;
    bool bBOC;

  protected:
    size_t lCurrent;

    CIterator(CIterator const &rhs) : bEOC(rhs.bEOC), bBOC(rhs.bBOC), lCurrent(rhs.lCurrent) {}

  public:
    CIterator() {}
    virtual ~CIterator() {}

    virtual  bool EOC()  { return bEOC; }
    virtual  bool BOC() const { return bBOC; }

    inline virtual void SetEOC() { bEOC = true; }
    inline virtual void SetBOC() { bBOC = true; }

    inline virtual void ResetEOC() { bEOC = false; }
    inline virtual void ResetBOC() { bBOC = false; }

    virtual void MoveFirst() = 0;
    virtual void MoveLast() = 0;
    virtual void MoveNext() = 0;
    virtual void MovePrevious() = 0;
  };
}

#endif // SCL_BASE_H
