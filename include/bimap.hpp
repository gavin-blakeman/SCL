#ifndef SCL_BIMAP_HPP
#define SCL_BIMAP_HPP

  // Standard C++ library header files.

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <unordered_map>

  // SCL Library header files

namespace SCL
{
  template <typename L, typename R> class bimap_iterator;

  /// @brief    The bimap class is a simple implementation of a bi-directional map type class. This relates a left hand side
  ///           (LHS) value to a right hand side (RHS) value. All the values must be unique. All the relationships are fixed and
  ///           1-1. The LHS:RHS pairs can be treated as pairs.
  ///           There container is also small value optimised, with different storage strategies utilised.

  template<class LHS_T,
           class RHS_T,
           bool SVO = (sizeof(LHS_T) <= sizeof(std::reference_wrapper<LHS_T>)) &&
                      (sizeof(RHS_T) <= sizeof(std::reference_wrapper<RHS_T>))
           >
  class bimap
  {
  public:
    friend class bimap_iterator<LHS_T, RHS_T>;

    using size_type = std::size_t;
    using value_type = std::pair<LHS_T, RHS_T>;
//    using left_type = std::pair<RHS_T, reference>;
//    using right_type = std::pair<LHS_T, reference>;
//    using pointer = typename std::allocator_traits<Allocator>::pointer;
//    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using const_iterator = bimap_iterator<LHS_T, RHS_T>;
//    using iterator_r = typename keyRight_storage::iterator;
//    using const_iterator_l = typename keyLeft_storage::const_iterator;
//    using reverse_iterator_l = typename keyLeft_storage::reverse_iterator;
//    using const_reverse_iterator_l = typename keyLeft_storage::const_reverse_iterator;

  private:
    constexpr static bool optimised = SVO;
    std::conditional<SVO,
                     std::unordered_map<LHS_T, RHS_T>,
                     std::unordered_map<LHS_T, std::reference_wrapper<value_type const>>>::type lhsMap;
    std::conditional<SVO,
                     std::unordered_map<RHS_T, LHS_T>,
                     std::unordered_map<RHS_T, std::reference_wrapper<value_type const>>>::type rhsMap;;


    struct empty_t{};

    [[no_unique_address]] std::conditional_t<SVO, empty_t, std::list<value_type>> valueStorage;


  public:
    bimap() = default;

    //template< class InputIt >
    //bimap(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator() );

    //template< class InputIt >
    //bimap(InputIt first, InputIt last, const Allocator& alloc );

    bimap(const bimap& other) = default;
    bimap(bimap&& other);
    bimap( std::initializer_list<std::pair<LHS_T, RHS_T>> init)
    {
      for (auto &v: init)
      {
        insert(v.first, v.second);
      }
    }

    ~bimap() = default;

    bimap& operator=(bimap const &other) = default;
    bimap& operator=(bimap&& other ) noexcept;
    bimap& operator=(std::initializer_list<value_type> ilist);

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  keyRight: The right key.
    /// @returns    const reference to the left key.
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.
    /// @version    2023-11-17/GGB - Function created.

    LHS_T const &LHS(RHS_T const &keyRight) const
    {
      if constexpr (SVO)
      {
        return rhsMap.at(keyRight);
      }
      else
      {
        return rhsMap.at(keyRight).get().first;
      }
    }

    /// @brief      Returns a reference to the mapped value of the element with key equivalent to key.
    ///             If no such element exists, an exception of type std::out_of_range is thrown.
    /// @param[in]  keyLeft: The left key of the element to find.
    /// @returns    Reference to the mapped value of the requested element
    /// @throws     std::out_of_range if the container does not have an element with the specified key
    /// @complexity Logarithmic in the size of the container.
    /// @version    2023-11-17/GGB - Function created.

    RHS_T const &RHS(LHS_T const &keyLeft) const
    {
      if constexpr (SVO)
      {
        return lhsMap.at(keyLeft);
      }
      else
      {
        return lhsMap.at(keyLeft).get().second;
      }
    }

    /// @brief      Checks if the container has no elements, i.e. whether begin() == end().
    /// @returns    true if the container is empty, false otherwise
    /// @throws     None.
    /// @version    2020-09-05/GGB - Function created.

    [[nodiscard]] bool empty() const noexcept
    {
      return lhsMap.empty();
    }

    /// @brief      Returns the number of elements in the container, i.e. std::distance(begin(), end()).
    /// @returns    The number of elements in the container.
    /// @throws     None.
    /// @version    2023-12-04/GGB - Function created.

    [[nodiscard]] size_type size() const noexcept
    {
      return lhsMap.size();
    }

    /// @brief      Erases all elements from the container. After this call, size() returns zero.
    /// @throws     None.
    /// @note       Invalidates any references, pointers, or iterators referring to contained elements. Any past-the-end iterator
    ///             remains valid.
    /// @version    2020-09-05/GGB - Function created.

    void clear() noexcept
    {
      lhsMap.clear();
      rhsMap.clear();
      if constexpr (!SVO)
      {
        valueStorage.clear();
      };
    }

    /// @brief      Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent
    ///             key.
    /// @param[in]  lk: The LHS key
    /// @param[in]  rk: The RHS key
    /// @complexity O(n)
    /// @throws
    /// @version    2020-09-05/GGB - Function created.

    void insert(LHS_T const &lk, RHS_T const &rk)
    {
      if constexpr (SVO)
      {
        lhsMap.emplace(lk, rk);
        rhsMap.emplace(rk, lk);
      }
      else
      {
        valueStorage.emplace_back(lk, rk);
        lhsMap.emplace(lk, std::cref(valueStorage.back()));
        rhsMap.emplace(rk, std::cref(valueStorage.back()));
      };
    }

    void insert(value_type v)
    {
      insert(v.first, v.second);
    }

    template<class InputIt>
    void insert(InputIt first, InputIt last)
    {
      std::for_each(first, last, [&](value_type const &v)
      {
        insert(v);
      });
    }

    void insert(std::initializer_list<std::pair<LHS_T, RHS_T>> ilist)
    {
      for (auto &v: ilist)
      {
        insert(v.first, v.second);
      }
    }

    [[nodiscard]] bool contains_LHS(LHS_T const &lk) const noexcept
    {
      return lhsMap.contains(lk);
    }

    [[nodiscard]] bool contains_RHS(RHS_T  const &rk) const noexcept
    {
      return rhsMap.contains(rk);
    }

    const_iterator begin() { return const_iterator(*this, lhsMap.begin());}
    const_iterator end() { return const_iterator(*this, lhsMap.end()); }

  };  // class bimap

  template<typename L,
           typename R>
  class bimap_iterator
  {
  public:
    using iteratored_type = bimap<L, R>;
    using value_type = iteratored_type::value_type;
    using internal_iterator = decltype(bimap<L, R>::lhsMap)::const_iterator;
    using const_pointer = value_type const *;
    using const_reference = value_type const &;
    using iterator_category = std::input_iterator_tag;

    bimap_iterator() = delete;
    bimap_iterator(bimap<L, R> const &i, internal_iterator ii) : instance(i), internalIterator(ii) {}
    bimap_iterator(bimap_iterator const &i) = default;

    const_reference operator*() const
    {
      if constexpr (instance.optimised)
      {
        currentValue = *internalIterator;
      }
      else
      {
        currentValue = *internalIterator.get();
      }
      return currentValue;
    }

    const_pointer operator->() const
    {
      if constexpr (instance.optimised)
      {
        currentValue = *internalIterator;
      }
      else
      {
        currentValue = *internalIterator.get();
      }
      return &currentValue;
    }


    bimap_iterator &operator++()
    {
      ++internalIterator;
      return *this;
    }

    bimap_iterator &operator++(int)
    {
      bimap_iterator temp(*this);

      ++internalIterator;
      return temp;
    }

    friend bool operator==(bimap_iterator const &lhs, bimap_iterator const rhs)
    {
      return lhs.internalIterator == rhs.internalIterator;
    }

    friend bool operator!=(bimap_iterator const &lhs, bimap_iterator const rhs)
    {
      return lhs.internalIterator != rhs.internalIterator;
    }


  private:
    bimap<L, R> const &instance;
    internal_iterator internalIterator;
    mutable value_type currentValue;
  };

} // namespace SCL

#endif // SCL_BIMAP_HPP
