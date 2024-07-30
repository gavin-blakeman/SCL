#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include "include/vectorSorted.hpp"

BOOST_AUTO_TEST_SUITE(vectorSorted)

BOOST_AUTO_TEST_CASE(constructors)
{
  SCL::vector_sorted<int> vsi;

  SCL::vector_sorted<std::string> vss;

  SCL::vector_sorted<int> vsi2 = {1, 3, 4, 5, 6, 7, 8};
  BOOST_TEST(vsi2.size() == 7);

  SCL::vector_sorted<std::string> vss2 = {"H", "ELLO", "TRY", "this" };
  BOOST_TEST(vss2.size() == 4);

//  vsi = vsi2;  // Copy constructor.
//  BOOST_TEST(vsi.size() == 7);

//  vss = std::move(vss2);  // Move constructor.
//  BOOST_TEST(vss.size() == 4);
//  BOOST_TEST(vss2.empty());
}

BOOST_AUTO_TEST_CASE(inserters)
{
  
}

BOOST_AUTO_TEST_SUITE_END()
