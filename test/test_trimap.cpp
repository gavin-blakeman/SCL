/*
 * test_trimap.cpp
 *
 *  Created on: 25 Aug 2024
 *      Author: Gavin Blakeman
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include "include/trimap.hpp"
#include <GCL>


BOOST_AUTO_TEST_SUITE(trimap_testSuite)

BOOST_AUTO_TEST_CASE(trimap_insert)
{
  SCL::trimap<std::string, std::string,  std::string> testMap;
  BOOST_TEST(testMap.empty());

  testMap.insert(std::string("one"), std::string("1"), std::string("I"));
  testMap.insert(std::string("two"), std::string("2"), std::string("II"));
  testMap.insert(std::string("three"), std::string("3"), std::string("III"));

  BOOST_TEST(testMap.contains_LHS("one"));
  BOOST_TEST(!testMap.contains_CHS("one"));
  BOOST_TEST(!testMap.contains_RHS("one"));
  BOOST_TEST(!testMap.contains_LHS("1"));
  BOOST_TEST(testMap.contains_CHS("1"));
  BOOST_TEST(!testMap.contains_RHS("1"));
  BOOST_TEST(testMap.size() == 3);
  BOOST_TEST(!testMap.empty());
}

BOOST_AUTO_TEST_CASE(trimap_insert_iterator)
{
  SCL::trimap<int, int, int> testMap;
  BOOST_TEST(testMap.empty());

  std::vector<std::tuple<int, int, int>> values = { { 1, 10, 100}, {2, 20, 200}, {3, 30, 300} };
  testMap.insert(values.begin(), values.end());

  BOOST_TEST(testMap.contains_LHS(1));
  BOOST_TEST(!testMap.contains_LHS(10));
  BOOST_TEST(testMap.contains_CHS(20));
  BOOST_TEST(!testMap.contains_RHS(3));
  BOOST_TEST(testMap.size() == 3);
  BOOST_TEST(!testMap.empty());
}

BOOST_AUTO_TEST_CASE(trimap_emplace)
{
  SCL::trimap<std::string, std::string, std::string> testMap;
  BOOST_TEST(testMap.empty());

  testMap.emplace(std::string("one"), std::string("1"), std::string("I"));
  testMap.emplace(std::string("two"), std::string("2"), std::string("II"));
  testMap.emplace(std::string("three"), std::string("3"), std::string("III"));

  BOOST_TEST(testMap.contains_LHS("one"));
  BOOST_TEST(!testMap.contains_CHS("one"));
  BOOST_TEST(!testMap.contains_RHS("one"));
  BOOST_TEST(!testMap.contains_LHS("1"));
  BOOST_TEST(testMap.contains_CHS("1"));
  BOOST_TEST(!testMap.contains_RHS("1"));
  BOOST_TEST(testMap.size() == 3);
  BOOST_TEST(!testMap.empty());
}

BOOST_AUTO_TEST_SUITE_END()
