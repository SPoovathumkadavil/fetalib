
#include <string>
#include <vector>

#include <fetalib/cli/arguments.hpp>
#include <fetalib/common/util.hpp>
#include <gtest/gtest.h>
#include <stdio.h>

TEST(ArgTest, EnsureArgVCorrectlyTranscribed)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  ASSERT_TRUE(argparser.get_argc() == 2 && argparser.get_argv()->size() == 2);
}

TEST(ArgTest, EnsureArgExists)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  argparser.add_option("-a", "--aha");
  ASSERT_TRUE(argparser.arg_exists("-a") && argparser.arg_exists("--aha"));
}

TEST(ArgTest, EnsureArgValue)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  argparser.add_option("-a", "--aha");
  feta::Argument arg = argparser.get_arg("-a");
  ASSERT_TRUE(arg.key == std::string("-a")
              && arg.alternate_key == std::string("--aha"));
}

TEST(ArgTest, EnsureGetVecVal)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  argparser.add_option("-a", "--aha");
  std::vector<std::string> arg =
      argparser.get<std::vector<std::string>>("-a");
  ASSERT_TRUE(arg[0] == std::string("testa"));
}

TEST(ArgTest, EnsureGetStrVal)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  argparser.add_option("-a", "--aha");
  std::string arg = argparser.get<std::string>("-a");
  ASSERT_TRUE(arg == std::string("testa"));
}

TEST(ArgTest, EnsureGetIntVal)
{
  char* argv[] = {"--aha", "12"};
  feta::ArgumentParser argparser(2, argv);
  argparser.add_option("-a", "--aha");
  int arg = argparser.get<int>("-a");
  ASSERT_TRUE(arg == 12);
}

TEST(ArgTest, EnsureGetBoolVal)
{
  char* argv[] = {"--aha", "true"};
  feta::ArgumentParser argparser(2, argv);
  argparser.add_option("-a", "--aha");
  bool arg = argparser.get<bool>("-a");
  ASSERT_TRUE(arg);
}

TEST(ArgTest, EnsureGetFlagVal)
{
  char* argv[] = {"--aha"};
  feta::ArgumentParser argparser(1, argv);
  argparser.add_option("-a", "--aha", "", true);
  bool arg = argparser.get<bool>("-a");
  ASSERT_TRUE(arg);
}

TEST(ArgTest, EnsureArgvMultiDefValidation)
{
  char* argv[] = {"--aha", "testa", "-a"};
  feta::ArgumentParser argparser(3, argv);
  argparser.add_option("-a", "--aha");
  ASSERT_FALSE(argparser.validate().valid);
}

TEST(ArgTest, EnsureArgvNoValueValidation)
{
  char* argv[] = {"--aha", "-b", "testb"};
  feta::ArgumentParser argparser(3, argv);
  argparser.add_option("-a", "--aha");
  argparser.add_option("-b");
  ASSERT_FALSE(argparser.validate().valid);
}

TEST(ArgTest, EnsureArgvGoodValidation)
{
  char* argv[] = {"--aha", "testa", "-b", "testb"};
  feta::ArgumentParser argparser(4, argv);
  argparser.add_option("-a", "--aha");
  argparser.add_option("-b");
  ASSERT_TRUE(argparser.validate().valid);
}
