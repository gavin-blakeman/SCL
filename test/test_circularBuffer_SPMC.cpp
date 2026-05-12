
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <memory>
#include <random>

#include "include/circularBuffer_SPMC.hpp"
#include <GCL>

BOOST_AUTO_TEST_SUITE(circularBuffer_SPMC_test)

BOOST_AUTO_TEST_CASE(construct_and_destruct)
{
  using namespace SCL;

  std::unique_ptr<circular_buffer_SPMC<int, 1024>> intTest = std::make_unique<circular_buffer_SPMC<int, 1024>>();

  BOOST_CHECK_EQUAL(intTest->max_size(), 1023);
  BOOST_CHECK_EQUAL(intTest->size(), 0);


  circular_buffer_SPMC<char, 2000> charTest;
  BOOST_CHECK_EQUAL(charTest.max_size(), 2047);
  BOOST_CHECK_EQUAL(charTest.size(), 0);
}

BOOST_AUTO_TEST_CASE(push_copy)
{
  using namespace SCL;

  circular_buffer_SPMC<int, 3> intTest;
  BOOST_CHECK_EQUAL(intTest.max_size(), 3);

  BOOST_CHECK(intTest.push(1));
  BOOST_CHECK(intTest.push(2));
  BOOST_CHECK(intTest.push(3));
  BOOST_REQUIRE(!intTest.push(4));
  BOOST_CHECK_EQUAL(intTest.size(), 3);
}

BOOST_AUTO_TEST_CASE(pop)
{
  using namespace SCL;

  circular_buffer_SPMC<int, 4> intTest;

  intTest.push(1);
  intTest.push(2);
  intTest.push(3);

  int v;
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 1);
  BOOST_TEST(intTest.size() == 2);
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 2);
  BOOST_TEST(intTest.size() == 1);
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 3);
  BOOST_TEST(intTest.size() == 0);
  BOOST_TEST(!intTest.pop(v));
  BOOST_TEST(intTest.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
