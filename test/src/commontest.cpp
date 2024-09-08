
#include <fetalib/common/util.hpp>
#include <string>
#include <vector>
#include <gtest/gtest.h>

TEST(CommonTest, EnsureVecSubst)
{
  std::vector<std::string> vec;
  vec.push_back("hehe");
  vec.push_back("hehe");
  vec.push_back("hehe");
  vec.push_back("hehe");
  vec.push_back("hehe");
  int s1 = vec.size();
  vec = feta::util::vec_substring(&vec, 2, 3);
  int s2 = vec.size();
  ASSERT_TRUE(s2 > 0);
}

TEST(CommonTest, EnsureJoinVecSubst) {
  std::vector<std::string> vec;
  vec.push_back("hehe");
  vec.push_back("hehe");
  vec.push_back("hehe");
  vec.push_back("hehe");
  vec.push_back("hehe");
  int s1 = vec.size();
  std::string vstr = feta::util::join(feta::util::vec_substring(&vec, 2, 4), " ");
  ASSERT_EQ(vstr, std::string("hehe hehe"));
}