#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include "include/multiVectorSorted.hpp"

BOOST_AUTO_TEST_SUITE(multiVectorSorted)

BOOST_AUTO_TEST_CASE(insert)
{
  SCL::multiVector_sorted mvs;

  BOOST_TEST(MCL::mean(X) == 1.650);
  BOOST_TEST(MCL::mean(Y) == 62.078);

}

BOOST_AUTO_TEST_SUITE_END()
