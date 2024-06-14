#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <memory>
#include <random>

#include "include/circularBuffer.hpp"
#include <GCL>


BOOST_AUTO_TEST_SUITE(circularBuffer_test)

BOOST_AUTO_TEST_CASE(construct_and_destruct)
{
  using namespace SCL;

  std::unique_ptr<circularBuffer<int, 1024, true, true>> intTest = std::make_unique<circularBuffer<int, 1024, true, true>>();
  circularBuffer<char, 2048, false, false> charTest;

  BOOST_TEST(intTest->capacity() == 1024);
  BOOST_TEST(charTest.capacity() == 2048);

  intTest.reset();
}

BOOST_AUTO_TEST_CASE(push_and_pop)
{
  using namespace SCL;

  circularBuffer<int, 1024, true, true> intTest;

  intTest.push(1);
  intTest.push(2);
  intTest.push(3);
  intTest.push(4);

  BOOST_TEST(intTest.back() == 4);
  BOOST_TEST(intTest.front() == 1);

  BOOST_TEST(intTest.size() == 4);

  intTest.push(5);
  BOOST_TEST(intTest.size() == 5);
  BOOST_TEST(!intTest.empty());
  intTest.pop();
  intTest.pop();
  BOOST_TEST(intTest.size() == 3);

  intTest.clear();
  BOOST_TEST(intTest.empty());
  BOOST_TEST(intTest.size() == 0);
}

BOOST_AUTO_TEST_CASE(access_functions)
{
  using namespace SCL;

  std::vector<std::uint32_t> characters;
  circularBuffer<std::uint32_t, 1024, true, true> intTest;

  for (std::size_t cnt = 0; cnt != 1025; cnt++)
  {
    characters.push_back(cnt);
    intTest.push(characters.back());

    BOOST_TEST(intTest.back() == cnt);
  };

  BOOST_TEST(intTest.full());
  BOOST_TEST(!intTest.empty());
  BOOST_TEST(intTest.back() == 1024);

  BOOST_TEST(characters[3] == intTest[1]);
  BOOST_TEST(characters[300] == intTest[298]);

  for (std::size_t cnt = 0; cnt != 512; cnt++)
  {
    intTest.pop();
  }

  for (std::size_t cnt = 1025; cnt != 1281; cnt++)
  {
    characters.push_back(cnt);
    intTest.push(characters.back());
  }

  BOOST_TEST(intTest.size() == (1024 - 512 + 256));
  BOOST_TEST(intTest[345] == characters[1026-512+345]);
  BOOST_TEST(intTest[600] == characters[1026-512+600]);
}



BOOST_AUTO_TEST_SUITE_END()
