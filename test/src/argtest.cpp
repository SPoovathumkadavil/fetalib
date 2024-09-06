
#include <fetalib/common/util.hpp>
#include <fetalib/cli/arguments.hpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <gtest/gtest.h>

TEST(ArgTest, EnsureArgumentFindKey)
{
  char* argv[] = {"-a", "testa", "-b", "testb"};
  feta::ArgumentParser argparser(4, argv);
  argparser.add_option("-a");
  argparser.add_option("-b");
  ASSERT_TRUE(argparser.arg_exists("-a"));
}

TEST(ArgTest, EnsureArgumentGetKeyVal)
{
  char* argv[] = {"-a", "testa", "-b", "testb"};
  feta::ArgumentParser argparser(4, argv);
  argparser.add_option("-a");
  argparser.add_option("-b");
  std::string str = argparser.get_vec_val("-a")[0];
  ASSERT_EQ(str, std::string("testa"));
}

TEST(CommonTest, EnsureVecSubstring)
{
  std::vector<std::string> vec;
  vec.push_back("hehe");
  vec.push_back("hehe");
  vec.push_back("hehe");
  vec.push_back("hehe");
  vec.push_back("hehe");
  int s1 = vec.size();
  vec = feta::util::vec_substring(vec, 2, 3);
  int s2 = vec.size();
  ASSERT_TRUE(s2 > 0);
}

TEST(ArgTest, EnsureArgV) {
  std::printf("\nTEST OUTPUT\n");
  char* argv[] = {"-a", "testa", "-b", "testb"};
  feta::ArgumentParser argparser(4, argv);
  for (std::string str : argparser._argv) {
    std::printf("%s\n", str.c_str());
  }
  std::printf("\n\t- vecsub argv:\n");
  // std::vector<std::string>  argvstr = feta::util::vec_substring(argparser._argv, 0+1, 0+1+1);
  // for (std::string str : argvstr) {
  //   std::printf("%s\n", str.c_str());
  // }
  std::printf("%s\n", feta::util::join(feta::util::vec_substring(argparser._argv, 0+1, 0+1+1), " ").c_str());
  std::printf("-----------\n");
}
