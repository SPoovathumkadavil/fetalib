
#include <string>
#include <vector>

#include <fetalib/cli/arguments.hpp>
#include <fetalib/common/util.hpp>
#include <gtest/gtest.h>
#include <stdio.h>

TEST(ArgTest, EnsureSimpleArgFind)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg = feta::get_blank_argument();
  arg.key = "-a";
  argparser.add(arg);
  ASSERT_EQ(argparser.get<std::string>("-a").value(), std::string("testa"));
}

TEST(ArgTest, EnsureErrorOnMissingArg)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg = feta::get_blank_argument();
  arg.key = "-a";
  argparser.add(arg);
  EXPECT_THROW(argparser.get<std::string>("-b").value(), std::invalid_argument);
}

TEST(ArgTest, EnsureChainingArgumentCreation)
{
  char* argv[] = {"--meow", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg = feta::get_blank_argument().withKey("-m").withAlternateKey("--meow");
  argparser.add(arg);
  ASSERT_EQ(argparser.get<std::string>(arg).value(), std::string("testa"));
}

TEST(ArgTest, EnsurePositiveValidation)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg = feta::get_blank_argument().withKey("-a");
  argparser.add(arg);
  ASSERT_TRUE(argparser.validate().valid);
}


TEST(ArgTest, EnsureMissingArgValidation)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg = feta::get_blank_argument().withKey("-b").withOptional(false);
  argparser.add(arg);
  ASSERT_FALSE(argparser.validate().valid);
}

