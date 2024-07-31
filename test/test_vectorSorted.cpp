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

  vsi = vsi2;  // Copy constructor.
  BOOST_TEST(vsi.size() == 7);

//  vss = std::move(vss2);  // Move constructor.
//  BOOST_TEST(vss.size() == 4);
//  BOOST_TEST(vss2.empty());
}

BOOST_AUTO_TEST_CASE(empty)
{
  SCL::vector_sorted<int> sv0;
  BOOST_TEST(sv0.empty());

  SCL::vector_sorted<int> sv1 = { 0, 1};
  BOOST_TEST(!sv1.empty());
}

BOOST_AUTO_TEST_CASE(size)
{
  SCL::vector_sorted<int> sv0;
  BOOST_TEST(sv0.size() == 0);

  SCL::vector_sorted<int> sv1 = {0, 1};
  BOOST_TEST(sv1.size() == 2);
}

BOOST_AUTO_TEST_CASE(clear)
{
  SCL::vector_sorted<int> sv0 = {0, 1, 2};

  BOOST_TEST(!sv0.empty());
  BOOST_TEST(sv0.size() == 3);

  sv0.clear();

  BOOST_TEST(sv0.empty());
  BOOST_TEST(sv0.size() == 0);
}

BOOST_AUTO_TEST_CASE(insert)
{
  SCL::vector_sorted<int> sv0;

  sv0.insert(4);
  sv0.insert(1);
  sv0.insert(2);

  BOOST_TEST(!sv0.empty());
  BOOST_TEST(sv0.size() == 3);

  BOOST_TEST(sv0[0] == 1);
  BOOST_TEST(sv0[1] == 2);
  BOOST_TEST(sv0[2] == 4);
}

BOOST_AUTO_TEST_CASE(emplace)
{
  SCL::vector_sorted<int> sv0;

  sv0.emplace(4);
  sv0.emplace(1);
  sv0.emplace(2);

  BOOST_TEST(!sv0.empty());
  BOOST_TEST(sv0.size() == 3);

  BOOST_TEST(sv0[0] == 1);
  BOOST_TEST(sv0[1] == 2);
  BOOST_TEST(sv0[2] == 4);
}

BOOST_AUTO_TEST_CASE(beginEnd)
{
  SCL::vector_sorted<std::string> sv1;
 // BOOST_TEST(sv1.begin() == sv1.end());

  SCL::vector_sorted<int> sv0 = { 0, 2, 1, 10, 9, 5, 4, 7, 6, 3, 8 };
  std::vector<int> res = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  std::size_t indx = 0;
  auto iter = sv0.begin();
  while (iter != sv0.end())
  {
    BOOST_TEST(res[indx++] == *iter++);
  };
}

BOOST_AUTO_TEST_CASE(find)
{
  SCL::vector_sorted<int> sv0 = { 0, 2, 1, 10, 9, 5, 4, 7, 6, 8, 3 };

  bool res = (*sv0.find(3) == 3);
  BOOST_TEST(res);

  res = (sv0.find(11) == sv0.end());
  BOOST_TEST(res);
}

BOOST_AUTO_TEST_CASE(contains)
{
  SCL::vector_sorted<int> sv0 = { 0, 2, 1, 10, 9, 5, 4, 7, 6, 8 };

  BOOST_TEST(sv0.contains(2));
  BOOST_TEST(sv0.contains(10));
  BOOST_TEST(sv0.contains(0));
  BOOST_TEST(!sv0.contains(11));
  BOOST_TEST(!sv0.contains(3));
}

BOOST_AUTO_TEST_CASE(pushBack)
{
  SCL::vector_sorted<int> sv0 = { 0, 2, 1, 10, 9, 5, 4, 7, 6, 8 };

  BOOST_TEST(sv0.back() == 10);
  sv0.push_back(3);
  BOOST_TEST(sv0.back() == 10);
  sv0.push_back(11);
  BOOST_TEST(sv0.back() == 11);
}

BOOST_AUTO_TEST_CASE(popBack)
{
  SCL::vector_sorted<int> sv0 = { 0, 2, 1, 10, 9, 5, 4, 7, 6, 8 };

  BOOST_TEST(sv0.back() == 10);
  sv0.pop_back();
  BOOST_TEST(sv0.back() == 9);
  sv0.pop_back();
  BOOST_TEST(sv0.back() == 8);
}

BOOST_AUTO_TEST_CASE(frontBack)
{
  SCL::vector_sorted<int> sv0 = { 0, 2, 1, 10, 9, 5, 4, 7, 6, 8 };

  BOOST_TEST(sv0.front() == 0);
  BOOST_TEST(sv0.back() == 10);

  sv0.emplace(11);
  BOOST_TEST(sv0.back() == 11);
}

BOOST_AUTO_TEST_CASE(erase)
{
  SCL::vector_sorted<int> sv0 = { 0, 2, 1, 10, 9, 5, 4, 7, 6, 8, 3 };

  BOOST_TEST(sv0.front() == 0);
  BOOST_TEST(sv0.back() == 10);
  BOOST_TEST(sv0.size() == 11);

  sv0.erase(3);
  BOOST_TEST(sv0.front() == 0);
  BOOST_TEST(sv0.back() == 10);
  BOOST_TEST(sv0.size() == 10);

  sv0.erase(0);
  BOOST_TEST(sv0.front() == 1);
  BOOST_TEST(sv0.back() == 10);
  BOOST_TEST(sv0.size() == 9);

  sv0.erase(10);
  BOOST_TEST(sv0.front() == 1);
  BOOST_TEST(sv0.back() == 9);
  BOOST_TEST(sv0.size() == 8);

}


BOOST_AUTO_TEST_SUITE_END()
