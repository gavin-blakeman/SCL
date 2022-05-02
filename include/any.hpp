//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                any.hpp
// SUBSYSTEM:           SCL::any
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2015-2022 Gavin Blakeman.
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
// --- Copyright notice below from the std::any file that was used as a reference implementation.
//
// Copyright (C) 2014-2018 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.
//
// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// OVERVIEW:            Implements SCL::any that is similar to std::any with some additional support for outputting the stored
//                      values as strings.
//
// HISTORY:             2020-09-23 GGB - Rename class and file to variant. Rewrite to do small object allocation.
//                      2020-05-05 GGB - Renamed file to any.hpp and class to any.
//                      2015-09-22 GGB - astroManager 2015.09 release
//                      2015-09-07 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef SCL_ANY_H
#define SCL_ANY_H

  // Standard C++ library header files.

#include <any>
#include <memory>
#include <initializer_list>
#include <string>
#include <typeinfo>
#include <type_traits>

  // Miscellaneous library header files

#include "boost/filesystem.hpp"

  // SCL header files

#include "error.h"

namespace SCL
{
  /// @brief    The SCL::any class allows any type of data to be stored.
  /// @details  The class implement small buffer optimisation. Any type of data can be stored. If stringification of the stored
  ///           data is required, then it may be necessary to specialise the class for non-standard classes.
  ///           The class mimics std::any. The
  ///
  /// @note 1.  The SCL::any class is necessary for GCL::sqlWriter functionality.
  /// @note 2.  Reference was made to std::any implementations to develop this class.
  class any
  {
    // Holds either a pointer to a heap allocated object or the contained object itself.

    union objectStorage
    {
      objectStorage() = default;    // This constructor intentionally doesn't initialize the data.

        // Prevent trivial copies of this type, buffer might hold a non-POD.
      objectStorage(const objectStorage&) = delete;
      objectStorage& operator=(const objectStorage&) = delete;

      void* heapPointer;
      std::aligned_storage<sizeof(heapPointer), alignof(void*)>::type memoryBuffer;
    };

    template<typename T, typename _Safe = std::is_nothrow_move_constructible<T>, bool _Fits = (sizeof(T) <= sizeof(objectStorage))
                                                                                           && (alignof(T) <= alignof(objectStorage))>
    using Internal = std::integral_constant<bool, _Safe::value && _Fits>;

    template<typename T>
    struct Manager_internal;   // uses small-object optimization

    template<typename T>
    struct Manager_external; // creates contained object on the heap

    template<typename T>
    using Manager = std::conditional_t<Internal<T>::value,
                                        Manager_internal<T>,
                                        Manager_external<T>>;

    template<typename T, typename Decayed = std::decay_t<T>>
    using Decay = std::enable_if_t<!std::is_same<Decayed, any>::value, Decayed>;

  public:
    /// @brief      Default constructor, creates an empty object.
    /// @version    2020-09-21/GGB - Added the M_toString() member.

    any() noexcept : M_manager(nullptr), M_toString(nullptr) { }

    /// @brief      Copy constructor, copies the state of @p other
    /// @param[in]  other: The class to copy.
    /// @version    2020-09-21/GGB - Added the M_toString() member.

    any(const any& other)
    {
      if (other.empty())
      {
        M_manager = nullptr;
        M_toString = nullptr;
      }
      else
      {
        Arg arg;
        arg.M_any_type = this;
        other.M_manager(Op_clone, &other, &arg);
      };
    }

    /// @brief      Move constructor, transfer the state from @p other
    /// @param[in]  other: The instance to move.
    /// @post       @c other.empty()
    /// @version    2020-09-21/GGB - Added the M_toString() member.

    any(any&& other) noexcept
    {
      if (other.empty())
      {
        M_manager = nullptr;
        M_toString = nullptr;
      }
      else
      {
        Arg arg;
        arg.M_any_type = this;
        other.M_manager(Op_xfer, &other, &arg);
      };
    }

    /// @brief      Construct with a copy of @p value as the contained object.
    /// @param[in]  value: The value to intialise with.
    /// @version    2020-09-21/GGB - Added the M_toString() member.

    template <typename ValueType, typename T = Decay<ValueType>,
              typename Mgr = Manager<T>,
              typename std::enable_if<std::is_constructible<T, ValueType&&>::value, bool>::type = true>
    any(ValueType&& value)
      : M_manager(&Mgr::S_manage), M_toString(&Mgr::S_toString)
    {
      Mgr::S_create(dataStorage, std::forward<ValueType>(value));
      static_assert(std::is_copy_constructible<T>::value, "The contained object must be CopyConstructible");
    }

    /// @brief      Construct with a copy of @p value as the contained object.
    /// @param[in]  value: The value to intialise with.
    /// @version    2020-09-21/GGB - Added the M_toString() member.

    template <typename ValueType, typename T = Decay<ValueType>,
              typename _Mgr = Manager<T>,
              typename std::enable_if<!std::is_constructible<T, ValueType&&>::value, bool>::type = false>
    any(ValueType&& value)
      : M_manager(&_Mgr::S_manage), M_toString(&_Mgr::S_toString)
    {
      _Mgr::_S_create(dataStorage, value);
      static_assert(std::is_copy_constructible<T>::value, "The contained object must be CopyConstructible");
    }

    /// @brief      Destructor, calls @c clear()
    ///
    ~any()
    {
      clear();
    }

    // assignments

    /// @brief      Copy the state of another object.
    /// @param[in]  rhs: The value to copy.
    ///

    any& operator=(const any& rhs)
    {
      *this = any(rhs);
      return *this;
    }

    /// @brief      Move assignment operator
    /// @param[in]  rhs: The value to move.
    /// @post       @c rhs.empty()
    /// @version    2020-09-21/GGB - Added the M_toString() member.

    any& operator=(any&& rhs) noexcept
    {
      if (rhs.empty())
      {
        clear();
      }
      else if (this != &rhs)
      {
        clear();
        Arg arg;
        arg.M_any_type = this;
        rhs.M_manager(Op_xfer, &rhs, &arg);     // Copies the string function pointer.
      }
      return *this;
    }

    /// Store a copy of @p rhs as the contained object.
    /// @param[in]  rhs: The value to move.
    /// @post       @c rhs.empty() (not guaranteed for other implementations)
    /// @version    2020-09-21/GGB - Added the M_toString() member.

    template<typename ValueType>
    std::enable_if_t<!std::is_same<any, std::decay_t<ValueType>>::value, any&>
    operator=(ValueType&& rhs)
    {
      *this = any(std::forward<ValueType>(rhs));
      return *this;
    }

    /// @brief      If not empty, destroy the contained object.
    /// @version    2020-09-21/GGB - Added the M_toString() member.

    void clear() noexcept
    {
      if (!empty())
      {
        M_manager(Op_destroy, this, nullptr);
        M_manager = nullptr;
        M_toString = nullptr;
      }
    }

    /// @brief      Exchange state with another object.
    /// @param[in]  rhs: The instance to swap
    /// @version    2020-09-21/GGB - Added the M_toString() member.

    void swap(any& rhs) noexcept
    {
      if (empty() && rhs.empty())
      {
        return;
      };

      if (!empty() && !rhs.empty())
      {
        if (this == &rhs)
        {
          return;
        };

        any tmp;
        Arg arg;
        arg.M_any_type = &tmp;
        rhs.M_manager(Op_xfer, &rhs, &arg);
        arg.M_any_type = &rhs;
        M_manager(Op_xfer, this, &arg);
        arg.M_any_type = this;
        tmp.M_manager(Op_xfer, &tmp, &arg);
      }
      else
      {
        any* empty = this->empty() ? this : &rhs;
        any* full = this->empty() ? &rhs : this;
        Arg arg;
        arg.M_any_type = empty;
        full->M_manager(Op_xfer, full, &arg);
      };
    }

    // observers

    /// Reports whether there is a contained object or not.
    ///

    bool empty() const noexcept
    {
      return M_manager == nullptr;
    }

    /// The @c typeid of the contained object, or @c typeid(void) if empty.
    const std::type_info& type() const noexcept
    {
      if (empty())
      {
        return typeid(void);
      }
      else
      {
        Arg arg;
        M_manager(Op_get_type_info, this, &arg);
        return *arg.M_typeinfo;
      };
    }

    //       template<typename _B1, typename _B2>
    //         struct __or_<_B1, _B2>
    //         : public conditional<_B1::value, _B1, _B2>::type
    //         { };

    //       template<typename _Tp>
    //         static constexpr bool __is_valid_cast()
    //         { return __or_<std::is_reference<_Tp>, std::is_copy_constructible<_Tp>>::value; }

    template<typename _Tp>
    static constexpr bool __is_valid_cast()
    {
      return std::conditional<std::is_reference<_Tp>::value,
                              std::is_reference<_Tp>,
                              std::is_copy_constructible<_Tp>>::type::value;
    }

    /// @brief      Converts the stored value to a string by calling the std::to_string(class) on the stored object. The
    ///             std::to_string() function must be defined for the class.
    /// @returns    The internal object value converted to a string, or an empty string.
    /// @version    2020-09-21/GGB - Function created.

    std::string to_string() const
    {
      if (empty())
      {
        return std::string("");
      }
      else
      {
        return std::move(M_toString(this));
      };
    }


  private:
    enum Op
    {
      Op_access,
      Op_get_type_info,
      Op_clone,
      Op_destroy,
      Op_xfer
    };

    union Arg
    {
      void* M_obj;
      const std::type_info* M_typeinfo;
      any* M_any_type;
    };

    void (*M_manager)(Op, any const *, Arg*);
    std::string (*M_toString)(any const *);
    objectStorage dataStorage;

    template<typename T>
    friend void* __variant_t_caster(const any*);

    // Manage in-place contained object.
    template<typename T>
    struct Manager_internal
    {
      static void S_manage(Op which, const any *, Arg *);

      template<typename U>
      static void S_create(objectStorage& storage, U&& value)
      {
        void* addr = &storage.memoryBuffer;
        ::new (addr) T(std::forward<U>(value));
      }

      static std::string S_toString(const any *);
    };

    // Manage heap stored object.

    template<typename T>
    struct Manager_external
    {
      static void S_manage(Op, const any *, Arg *);

      template<typename U>
      static void S_create(objectStorage& storage, U&& value)
      {
        storage.heapPointer = new T(std::forward<U>(value));
      }

      static std::string S_toString(const any *);
    };
  };

  /// Exchange the states of two @c variant_t objects.
  inline void swap(any &x, any &y) noexcept
  {
    x.swap(y);
  }

  /// @brief      Access the contained object.
  /// @tparam     ValueType: A const-reference or CopyConstructible type.
  /// @param      variant_t:  The object to access.
  /// @returns    The contained object.
  /// @throw      bad_variant_t_cast

  template<typename ValueType>
  inline ValueType variant_t_cast(const any& variant)
  {
    static_assert(any::__is_valid_cast<ValueType>(), "Template argument must be a reference or CopyConstructible type");
    auto p = variant_t_cast<std::add_const_t<std::remove_reference_t<ValueType>>>(&variant);
    if (p)
    {
      return *p;
    }
    else
    {
      throw std::bad_cast("any cast to non-contained type");
    };
  }

  /// @brief      Access the contained object.
  /// @tparam     ValueType: A reference or CopyConstructible type.
  /// @param      variant_t: The object to access.
  /// @returns    The contained object.
  /// @throw      bad_variant_t_cast
  ///
  template<typename ValueType>
  inline ValueType variant_t_cast(any& variant)
  {
    static_assert(any::__is_valid_cast<ValueType>(), "Template argument must be a reference or CopyConstructible type");
    auto p = variant_t_cast<std::remove_reference_t<ValueType>>(&variant);
    if (p)
    {
      return *p;
    }
    else
    {
      throw std::bad_cast("any cast to non-contained type");
    };
  }

  template<typename _ValueType,
           typename std::enable_if<!std::is_move_constructible<_ValueType>::value
                                   || std::is_lvalue_reference<_ValueType>::value,
                                   bool>::type = true>
  inline _ValueType variant_t_cast(any&& __variant_t)
  {
    static_assert(any::__is_valid_cast<_ValueType>(), "Template argument must be a reference or CopyConstructible type");
    auto __p = variant_t_cast<std::remove_reference_t<_ValueType>>(&__variant_t);
    if (__p)
    {
      return *__p;
    }
    else
    {
      throw std::bad_cast("any cast to non-contained type");
    };
  }

  template<typename _ValueType,
           typename std::enable_if<std::is_move_constructible<_ValueType>::value
                                   && !std::is_lvalue_reference<_ValueType>::value,
                                   bool>::type = false>
  inline _ValueType variant_t_cast(any&& __variant_t)
  {
    static_assert(any::__is_valid_cast<_ValueType>(), "Template argument must be a reference or CopyConstructible type");
    auto __p = variant_t_cast<std::remove_reference_t<_ValueType>>(&__variant_t);
    if (__p)
    {
      return std::move(*__p);
    }
    else
    {
      throw std::bad_cast("any cast to non-contained type");
    };
  }

  template<typename T>
  void* any_caster(const any* any_type)
  {
    struct _None { };
    using U = std::decay_t<T>;
    using V = std::conditional_t<std::is_copy_constructible<U>::value, U, _None>;
    if (any_type->M_manager != &any::Manager<V>::_S_manage)
    {
      return nullptr;
    }
    else
    {
      any::Arg arg;
      any_type->M_manager(any::Op_access, any_type, &arg);
      return arg.M_obj;
    };
  }

  /// @brief      Access the contained object.
  /// @tparam     ValueType: The type of the contained object.
  /// @param      variant_t: A pointer to the object to access.
  /// @returns    The address of the contained object

  template<typename ValueType>
  inline const ValueType* any_cast(const any* any_type) noexcept
  {
    if (any_type)
    {
      return static_cast<ValueType*>(any_caster<ValueType>(any_type));
    }
    else
    {
      return nullptr;
    };
  }

  template<typename ValueType>
  inline ValueType* any_cast(any* any_type) noexcept
  {
    if (any_type)
    {
      return static_cast<ValueType*>(any_caster<ValueType>(any_type));
    }
    else
    {
      return nullptr;
    };
  }

  /// @brief        Templated String conversion function. This operates on a locally stored object.
  /// @tparam       T: Template type
  /// @param[in]    any_type: Pointer to the instance to operate on
  /// @returns      A std::string representing the string type.
  /// @version      2020-09-22/GGB - Function created.

  template<typename T>
  std::string any::Manager_internal<T>::S_toString(any const *any_type)
  {
      // The object is stored in _M_storage.memoryBuffer.

    auto ptr = reinterpret_cast<const T *>(&any_type->dataStorage.memoryBuffer);

    return std::move(std::to_string(*ptr));
  }

  /// @brief        Template instance for a stored 'char const *'
  /// @param[in]    variant_tp: Pointer to the instance to operate on
  /// @returns      A std::string representing the string type.
  /// @version      2020-09-22/GGB - Function created.

  template<>
  inline std::string any::Manager_internal<char const *>::S_toString(any const *variant_tp)
  {
    auto ptr = reinterpret_cast<char const *>(&variant_tp->dataStorage.memoryBuffer);

    return std::move(std::string(ptr));
  }


  template<typename T>
  void any::Manager_internal<T>::S_manage(Op which, const any* variant, Arg* arg)
  {
    // The contained object is in _M_storage.memoryBuffer
    auto ptr = reinterpret_cast<const T*>(&variant->dataStorage.memoryBuffer);
    switch (which)
    {
      case Op_access:
      {
        arg->M_obj = const_cast<T*>(ptr);
        break;
      };
      case Op_get_type_info:
      {
        arg->M_typeinfo = &typeid(T);
        break;
      };
      case Op_clone:
      {
        ::new(&arg->M_any_type->dataStorage.memoryBuffer) T(*ptr);
        arg->M_any_type->M_manager = variant->M_manager;
        arg->M_any_type->M_toString = variant->M_toString;
        break;
      };
      case Op_destroy:
      {
        ptr->~T();
        break;
      };
      case Op_xfer:
      {
        ::new(&arg->M_any_type->dataStorage.memoryBuffer) T(std::move(*const_cast<T*>(ptr)));
        ptr->~T();
        arg->M_any_type->M_manager = variant->M_manager;
        arg->M_any_type->M_toString = variant->M_toString;
        const_cast<any*>(variant)->M_manager = nullptr;
        break;
      };
    }
  }

  /// @brief        Templated String conversion function. This operates on a heap stored object.
  /// @tparam       T: Template type
  /// @param[in]    variant_tp: Pointer to the instance to operate on
  /// @returns      A std::string representing the string type.
  /// @version      2020-09-22/GGB - Function created.

  template<typename T>
  std::string any::Manager_external<T>::S_toString(any const *variant_tp)
  {
    auto ptr = static_cast<T const *>(variant_tp->dataStorage.heapPointer);

    return std::move(std::to_string(*ptr));
  }

  /// @brief        String conversion function for a 'char const *'. This operates on a heap stored object.
  /// @tparam       T: Template type
  /// @param[in]    variant_tp: Pointer to the instance to operate on
  /// @returns      A std::string representing the string type.
  /// @version      2020-09-22/GGB - Function created.

  template<>
  inline std::string any::Manager_external<char const *>::S_toString(any const *variant_tp)
  {
    auto ptr = static_cast<char const *>(variant_tp->dataStorage.heapPointer);

    return std::move(std::string(ptr));
  }

  template<>
  inline std::string any::Manager_external<boost::filesystem::path>::S_toString(any const *variant_tp)
  {
    auto ptr = static_cast<boost::filesystem::path const *>(variant_tp->dataStorage.heapPointer);

    return ptr->native();
  }

  /// @brief        String conversion function for a 'std::string'. This operates on a heap stored object.
  /// @tparam       T: Template type
  /// @param[in]    variant_tp: Pointer to the instance to operate on
  /// @returns      A std::string representing the string type.
  /// @version      2020-09-22/GGB - Function created.

  template<>
  inline std::string any::Manager_external<std::string>::S_toString(any const *variant_tp)
  {
    auto ptr = static_cast<std::string const *>(variant_tp->dataStorage.heapPointer);
    return std::string(*ptr);
  }

  template<typename T>
  void any::Manager_external<T>::S_manage(Op which, const any* variant, Arg* arg)
  {
    // The contained object is *_M_storage._M_ptr
    auto ptr = static_cast<const T*>(variant->dataStorage.heapPointer);
    switch (which)
    {
      case Op_access:
      {
        arg->M_obj = const_cast<T*>(ptr);
        break;
      };
      case Op_get_type_info:
      {
        arg->M_typeinfo = &typeid(T);
        break;
      };
      case Op_clone:
      {
        arg->M_any_type->dataStorage.heapPointer = new T(*ptr);
        arg->M_any_type->M_manager = variant->M_manager;
        arg->M_any_type->M_toString = variant->M_toString;
        break;
      };
      case Op_destroy:
      {
        delete ptr;
        break;
      };
      case Op_xfer:
      {
        arg->M_any_type->dataStorage.heapPointer = variant->dataStorage.heapPointer;
        arg->M_any_type->M_manager = variant->M_manager;
        arg->M_any_type->M_toString = variant->M_toString;
        const_cast<any*>(variant)->M_manager = nullptr;
        break;
      };
    }
  }


} // namespace SCL

#endif // SCL_ANY_H
