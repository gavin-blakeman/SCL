#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <GCL>
#include "include/stackHierarchy.hpp"

BOOST_AUTO_TEST_SUITE(parentChild)

template<typename T>
struct testClass
{
  T ID;
  T parentID;

  template<int N>
  T get()
  {
    static_assert(N <= 1);

    if constexpr (N == 0)
    {
      return ID;
    }
    else if constexpr (N == 1)
    {
      return parentID;
    }
    else
    {
      CODE_ERROR();
    }
  }

  friend bool operator!=(testClass const &lhs, testClass const &rhs) { return lhs.ID != rhs.ID; }
  friend bool operator<(testClass const &lhs, testClass const &rhs) { return lhs.ID < rhs.ID; }
};

using testTuple_int = std::tuple<std::uint64_t, std::uint64_t>;

std::list<testClass<std::uint32_t>> testData_int =
{
  {
    { 7,  3 },  { 2, 1 },   { 30, 15 }, { 14, 7 },  { 8, 4 },   { 6, 3 }, { 13, 6 }, { 24, 12 }, { 26, 13 }, { 5, 2 },
    { 22, 11 }, { 29, 14 }, { 12, 6 },  { 1, 0 },   { 11, 5 },  { 27, 13 }, { 3, 1 }, { 15, 7 }, { 10, 5 },  { 16, 8 },
    { 18, 9 },  { 23, 11 }, { 17, 8 },  { 21, 10 }, { 28, 14 }, { 20, 10 }, { 9, 4 }, { 4, 2 },  { 25, 12 }, { 31, 15 },
    { 19, 9 },
  }
};

SCL::parentChild<std::uint32_t, testClass<std::uint32_t>, 0, 1, std::list<testClass<std::uint32_t>>> testInt(testData_int);

BOOST_AUTO_TEST_CASE(preOrder)
{
  std::vector<std::reference_wrapper<testClass<std::uint32_t>>> testVector =
      testInt.preOrder<std::vector<std::reference_wrapper<testClass<std::uint32_t>>>>(0);

  std::vector<std::uint32_t> resultVector = { 1, 2, 4,  8,  16, 17, 9,  18, 19, 5,  10, 20, 21, 11, 22, 23,
                                              3, 6, 12, 24, 25, 13, 26, 27, 7,  14, 28, 29, 15, 30, 31 };
  std::vector<std::uint32_t> testVector2;
  for (auto const &v: testVector)
  {
    testVector2.push_back(v.get().get<0>());
  }

  BOOST_TEST(testVector.size() == resultVector.size());
  BOOST_TEST(testVector2 == resultVector);
}

BOOST_AUTO_TEST_CASE(postOrder)
{
  std::vector<std::reference_wrapper<testClass<std::uint32_t>>> testVector =
      testInt.postOrder<std::vector<std::reference_wrapper<testClass<std::uint32_t>>>>(0);

  std::vector<std::uint32_t> resultVector = { 16, 17, 8,  18, 19, 9,  4, 20, 21, 10, 22, 23, 11, 5, 2,
                                              24, 25, 12, 26, 27, 13, 6, 28, 29, 14, 30, 31, 15, 7, 3, 1};

  std::vector<std::uint32_t> testVector2;
  for (auto const &v: testVector)
  {
    testVector2.push_back(v.get().get<0>());
  }

  BOOST_TEST(testVector2 == resultVector);
}

BOOST_AUTO_TEST_SUITE_END()
