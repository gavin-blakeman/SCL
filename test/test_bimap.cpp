/*
 * test_bimap.cpp
 *
 *  Created on: 9 May 2024
 *      Author: Gavin Blakeman
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <GCL>
#include "include/bimap.hpp"

BOOST_AUTO_TEST_SUITE(bimap_testSuite)

BOOST_AUTO_TEST_CASE(bimap_insert_SVO)
{
  SCL::bimap<std::uint16_t, std::uint16_t> testMap;
  BOOST_TEST(testMap.empty());

  testMap.insert(std::uint16_t(10), std::uint16_t(0));
  testMap.insert(std::uint16_t(9), std::uint16_t(1));
  testMap.insert(std::uint16_t(8), std::uint16_t(2));

  BOOST_TEST(testMap.contains_LHS(10));
  BOOST_TEST(testMap.contains_RHS(2));
  BOOST_TEST(testMap.size() == 3);
  BOOST_TEST(!testMap.empty());
}

BOOST_AUTO_TEST_CASE(bimap_insert)
{
//  SCL::bimap<std::string, std::string> testMap;
//  BOOST_TEST(testMap.empty());
//
//  testMap.insert(std::uint16_t(10), std::uint16_t(0));
//  testMap.insert(std::uint16_t(9), std::uint16_t(1));
//  testMap.insert(std::uint16_t(8), std::uint16_t(2));
//
//  BOOST_TEST(testMap.contains_LHS(10));
//  BOOST_TEST(testMap.contains_RHS(2));
//  BOOST_TEST(testMap.size() == 3);
//  BOOST_TEST(!testMap.empty());
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





BOOST_AUTO_TEST_SUITE_END()
