/*
 * test_bimap.cpp
 *
 *  Created on: 9 May 2024
 *      Author: Gavin Blakeman
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include "include/bimap.hpp"
#include <GCL>


BOOST_AUTO_TEST_SUITE(bimap_testSuite)

BOOST_AUTO_TEST_CASE(bimap_insert_SVO)
{
  SCL::bimap<std::uint16_t, std::uint16_t> testMap;
  BOOST_TEST(testMap.empty());

  testMap.insert(std::uint16_t(10), std::uint16_t(0));
  testMap.insert(std::uint16_t(9), std::uint16_t(1));
  testMap.insert(std::uint16_t(8), std::uint16_t(2));

  BOOST_TEST(testMap.contains_LHS(10));
  BOOST_TEST(!testMap.contains_LHS(2));
  BOOST_TEST(testMap.contains_RHS(2));
  BOOST_TEST(!testMap.contains_RHS(8));
  BOOST_TEST(testMap.size() == 3);
  BOOST_TEST(!testMap.empty());
}

BOOST_AUTO_TEST_CASE(bimap_insert)
{
  SCL::bimap<std::string, std::string> testMap;
  BOOST_TEST(testMap.empty());

  testMap.insert(std::string("one"), std::string("ten"));
  testMap.insert(std::string("two"), std::string("nine"));
  testMap.insert(std::string("three"), std::string("eight"));

  BOOST_TEST(testMap.contains_LHS("one"));
  BOOST_TEST(!testMap.contains_RHS("one"));
  BOOST_TEST(testMap.contains_RHS("ten"));
  BOOST_TEST(!testMap.contains_RHS("one"));
  BOOST_TEST(testMap.size() == 3);
  BOOST_TEST(!testMap.empty());
}

BOOST_AUTO_TEST_CASE(bimap_insert_iterator)
{
  SCL::bimap<int, int> testMap;
  BOOST_TEST(testMap.empty());

  std::vector<std::pair<int, int>> values = { { 1, 10}, {2, 20}, {3, 30} };
  testMap.insert(values.begin(), values.end());

  BOOST_TEST(testMap.contains_LHS(1));
  BOOST_TEST(!testMap.contains_LHS(10));
  BOOST_TEST(testMap.contains_RHS(20));
  BOOST_TEST(!testMap.contains_RHS(3));
  BOOST_TEST(testMap.size() == 3);
  BOOST_TEST(!testMap.empty());
}

BOOST_AUTO_TEST_CASE(bimap_emplace_SVO)
{
  SCL::bimap<std::uint16_t, std::uint16_t> testMap;
  BOOST_TEST(testMap.empty());

  testMap.emplace(std::uint16_t(10), std::uint16_t(0));
  testMap.emplace(std::uint16_t(9), std::uint16_t(1));
  testMap.emplace(std::uint16_t(8), std::uint16_t(2));

  BOOST_TEST(testMap.contains_LHS(10));
  BOOST_TEST(testMap.contains_RHS(2));
  BOOST_TEST(testMap.size() == 3);
  BOOST_TEST(!testMap.empty());
}

BOOST_AUTO_TEST_CASE(bimap_emplace)
{
  SCL::bimap<std::string, std::string> testMap;
  BOOST_TEST(testMap.empty());

  testMap.emplace(std::string("one"), std::string("ten"));
  testMap.emplace(std::string("two"), std::string("nine"));
  testMap.emplace(std::string("three"), std::string("eight"));

  BOOST_TEST(testMap.contains_LHS("one"));
  BOOST_TEST(!testMap.contains_RHS("one"));
  BOOST_TEST(testMap.contains_RHS("ten"));
  BOOST_TEST(!testMap.contains_RHS("one"));
  BOOST_TEST(testMap.size() == 3);
  BOOST_TEST(!testMap.empty());
}

BOOST_AUTO_TEST_SUITE_END()
