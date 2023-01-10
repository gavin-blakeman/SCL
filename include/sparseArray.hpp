#ifndef SPARSEARRAY_HPP
#define SPARSEARRAY_HPP

  // Standard C++ library

#include <map>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace SCL
{

  /* The sparse array class implements a sparse array with integer indices. This is not intended for mathematics but for storing
   * information for other purposes such as spacial usages.
   */

  template<typename I,
           typename T,
           typename T_nonconst>
  class sparseArrayIterator
  {
  public:
    using index_type = std::pair<I, I>;
    typedef typename T::elem_type                                 elem_type;
    typedef sparseArrayIterator<I, T, T_nonconst>                 self_type;
    typedef T                                                     sparseArrayType;
    typedef std::input_iterator_tag                               iterator_category;
    typedef typename sparseArrayType::value_type                  value_type;
    typedef typename sparseArrayType::size_type                   size_type;
    typedef typename sparseArrayType::pointer                     pointer;
    typedef typename sparseArrayType::const_pointer               const_pointer;
    typedef typename sparseArrayType::reference                   reference;
    typedef typename sparseArrayType::const_reference             const_reference;
    typedef typename sparseArrayType::difference_type             difference_type;


  private:
    sparseArrayType *sparseArray_;
    index_type index_;

  protected:
  public:
    friend class sparseArrayIterator<I, T const, T>;

    /// @brief Constructor for the hierarchyIterator class.
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    sparseArrayIterator() : sparseArray_(nullptr)
    {

    }

    /// @brief Constructor for the hierarchyIterator class.
    /// @param[in] heirarchy: The hierarchy to be iterated.
    /// @throws
    /// @version 2019-10-26/GGB - Function created.

    sparseArrayIterator(sparseArrayType *sa, bool end = false) : sparseArray_(sa)
    {
    }

    /// @brief Copy constructor. Needed for pushing/popping from the stack.

    sparseArrayIterator(sparseArrayIterator<I, T, T> const &other)
    {
      throw std::exception("sparseArrayIterator Copy constructor not implemented");
    }

    /// @brief Implements the indirection operator.
    /// @returns
    /// @throws
    /// @version 2022-12-12/GGB - Function created.

    elem_type operator*()
    {
    }

    /// @brief Implements the pointer to operator
    /// @returns A std::pair<size_type, elem_type>. The first value is the hierarchy level, the second value if the item value.
    /// @version 2022-12-12/GGB - Function created.

    elem_type operator->()
    {
      throw std::exception("sparseArrayIterator operator-> not implemented.");
    }

    /// @brief The ++ and -- operators are used for stepping through the array in the x direction.
    /// @returns (*this)
    /// @throws
    /// @version 2022-12-12/GGB - Function created.

    self_type &operator++()
    {

    }

    self_type operator++(int);
    self_type operator+(difference_type);
    self_type operator+=(difference_type);

    /// @brief Implements the == operator for the iterator class.
    /// @param[in] other: The iterator to test against.
    /// @returns true - The iterators are the same.
    /// @returns false - The iterators are not the same.

    bool operator==(const self_type &other) const
    {
      bool returnValue = false;

      return returnValue;
    }

    /// @brief Implements the != operator for the iterator class.
    /// @param[in] other: The iterator to test against.
    /// @returns true - The iterators are the same.
    /// @returns false - The iterators are not the same.

    bool operator!=(const self_type &other) const
    {
      return !(*this == other);
    }

    bool operator>(const self_type &other) const;
    bool operator>=(const self_type &other) const;
    bool operator<(const self_type &other) const;
    bool operator<=(const self_type &other) const;
  };    // class hierarchyIterator

  /*!
   * @brief The sparseArray class is used for stroring information in a sparse array. This is not optimised for mathematics, but
   * allows efficient iteration and searches for information. This is simpler than a k-D tree as the requirements are different.
   *
   * The index type I can be signed or unsigned integers.
   *
   * Each elem_type stored is stored at and (x,y) coordinate pair. The iterators are specifically written to allow points around,
   * or in orthoganal directions to be easily found.
   *
   */


  template<typename I,
           class T,
           class Alloc_ = std::allocator<T>>
  class sparseArray
  {
  public:
    using index_type = std::pair<I, I>;
    using returnType_t = std::tuple<I, I, T>;
    using elem_type = T;
    using allocator_type = Alloc_;
    using pointer = typename std::allocator_traits<std::allocator<T>>::pointer;
    using const_pointer = typename std::allocator_traits<std::allocator<T>>::const_pointer;
    using reference = elem_type &;
    using const_reference = elem_type const &;
    using difference_type = typename Alloc_::difference_type;
    using size_type = typename Alloc_::size_type;

    /// @brief Class constructor. Emulates a default constructor for a default constructable object.
    /// @param[in] rootValue: The index value to use as the root value.
    /// @throws
    /// @version 2022-12-12/GGB - Function created.

    explicit sparseArray() {}

    virtual ~sparseArray()
    {
      destroyAllElements();
    }

    reference &at(I x, I y)
    {
      if (x_.contains(x))
      {
        if (x_[x].contains(y))
        {
          return dataVector.at(x_[x][y]);
        }
      }
    }

    const_reference t(I x, I y) const
    {

    }

    /// @brief Clears the structure. Removes all elements from the structure.
    /// @throws None.
    /// @version 2022-12-12/GGB - Function created.

    void clear() noexcept
    {
      destroyAllElements();
    }

    /// @brief Inserts new data into the array.
    /// @param[in] x: The x-coordinate
    /// @param[in] y: The y-coordinate.
    /// @param[in] d: The data to insert.
    /// @throws
    /// @version 2022-12-12/GGB - Function created.

    void insert(I x, I y, T d)
    {
      dataVector.emplace_back(d);
      size_type indx = dataVector.size() - 1;

      x_[x][y] = indx;
      y_[y][x] = indx;
    }

  private:
    std::vector<T, Alloc_> dataVector;        ///< Stores all the data. The index in the array is stored in the maps to allow search.
    std::map<I, std::map<I, size_type>> x_;
    std::map<I, std::map<I, size_type>> y_;

    /// @brief Destroys all the allocated elements.
    /// @throws None.
    /// @version 2022-12-12/GGB - Function created.

    void destroyAllElements()
    {
      dataVector.clear();
      x_.clear();
      y_.clear();
    }

  protected:
  };
}

#endif // SPARSEARRAY_HPP
