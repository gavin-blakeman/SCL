
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <memory>
#include <random>

#include "include/circularBuffer_SPMC.hpp"
#include <GCL>

BOOST_AUTO_TEST_SUITE(circularBuffer_lf_test)

BOOST_AUTO_TEST_CASE(construct_and_destruct)
{
  using namespace SCL;

  std::unique_ptr<circular_buffer_SPMC<int, 1024, true>> intTest = std::make_unique<circular_buffer_SPMC<int, 1024, true>>();
  circular_buffer_SPMC<char, 2048, false> charTest;

  BOOST_TEST(intTest->capacity() == 1024);
  BOOST_TEST(charTest.capacity() == 2048);
  BOOST_TEST(intTest->size() == 0);
  BOOST_TEST(charTest.size() == 0);

  intTest.reset();
}

BOOST_AUTO_TEST_CASE(capacity)
{
	SCL::circular_buffer_SPMC<int, 4, true> intTest;
	BOOST_TEST(intTest.capacity() == 4);
}

BOOST_AUTO_TEST_CASE(max_size)
{
	SCL::circular_buffer_SPMC<int, 4, true> intTest;
	BOOST_TEST(intTest.max_size() == 4);
}

BOOST_AUTO_TEST_CASE(push_accept_when_full)
{
  using namespace SCL;

  circular_buffer_SPMC<int, 4, true> intTest;

  BOOST_TEST(intTest.push(1));
  BOOST_TEST(intTest.push(2));
  BOOST_TEST(intTest.push(3));
  BOOST_TEST(intTest.push(4));
  BOOST_TEST(intTest.capacity() == 4);
  BOOST_TEST(intTest.size() == 4);
  BOOST_TEST(intTest.push(5));
  BOOST_TEST(intTest.size() == 4);

  intTest.clear();
  BOOST_TEST(intTest.empty());
  BOOST_TEST(intTest.size() == 0);
  BOOST_TEST(intTest.capacity() == 4);
}

BOOST_AUTO_TEST_CASE(push_not_accept_when_full)
{
  using namespace SCL;

  circular_buffer_SPMC<int, 4, false> intTest;

  BOOST_TEST(intTest.push(1));
  BOOST_TEST(intTest.push(2));
  BOOST_TEST(intTest.push(3));
  BOOST_TEST(intTest.push(4));
  BOOST_TEST(intTest.capacity() == 4);
  BOOST_TEST(intTest.size() == 4);
  BOOST_TEST(!intTest.push(5));
  BOOST_TEST(intTest.size() == 4);

  int v;
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 1);
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 2);
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 3);
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 4);
  BOOST_TEST(!intTest.pop(v));

  intTest.clear();
  BOOST_TEST(intTest.empty());
  BOOST_TEST(intTest.size() == 0);
  BOOST_TEST(intTest.capacity() == 4);
}

BOOST_AUTO_TEST_CASE(pop)
{
  using namespace SCL;

  circular_buffer_SPMC<int, 4, true> intTest;

  BOOST_TEST(intTest.push(1));
  BOOST_TEST(intTest.push(2));
  BOOST_TEST(intTest.push(3));
  BOOST_TEST(intTest.push(4));
  BOOST_TEST(intTest.capacity() == 4);
  BOOST_TEST(intTest.size() == 4);

  int v;
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 1);
  BOOST_TEST(intTest.size() == 3);
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 2);
  BOOST_TEST(intTest.size() == 2);
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 3);
  BOOST_TEST(intTest.size() == 1);
  BOOST_TEST(intTest.pop(v));
  BOOST_TEST(v == 4);
  BOOST_TEST(intTest.size() == 0);
  BOOST_TEST(!intTest.pop(v));
}

BOOST_AUTO_TEST_SUITE_END()
