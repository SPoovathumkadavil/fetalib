
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
  argparser.add(&arg);
  ASSERT_EQ(argparser.get<std::string>("-a").value(), std::string("testa"));
}

TEST(ArgTest, EnsureNulloptOnMissingArg)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg = feta::get_blank_argument();
  arg.key = "-a";
  argparser.add(&arg);
  ASSERT_EQ(argparser.get<std::string>("-b"), std::nullopt);
}

TEST(ArgTest, EnsureChainingArgumentCreation)
{
  char* argv[] = {"--meow", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg =
      feta::get_blank_argument().withKey("-m").withAlternateKey("--meow");
  argparser.add(&arg);
  ASSERT_EQ(argparser.get<std::string>(arg).value(), std::string("testa"));
}

TEST(ArgTest, EnsurePositiveValidation)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg = feta::get_blank_argument().withKey("-a");
  argparser.add(&arg);
  ASSERT_TRUE(argparser.validate().valid);
}

TEST(ArgTest, EnsureMissingArgValidation)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg =
      feta::get_blank_argument().withKey("-b").withOptional(false);
  argparser.add(&arg);
  ASSERT_FALSE(argparser.validate().valid);
}

TEST(ArgTest, EnsureArgPrinting)
{
  char* argv[] = {"-a", "testa"};
  feta::ArgumentParser argparser(2, argv);
  feta::detail::Argument arg =
      feta::get_blank_argument()
          .withKey("-a")
          .withAlternateKey("--ahah")
          .withOptional(false)
          .withHelpMessage(
              "heelo this is a very long help string that I am trying to write "
              "to intentionally make it long for the sake of things.");
  feta::detail::ArgumentDependency dep = {
      "help_me_please",
      "this is another long kinda long message like i thought. now i will make "
      "it even longer because i want to see if it works."};
  feta::detail::Argument arg2 =
      feta::get_blank_argument()
          .withKey("-b")
          .withOptional(false)
          .withHelpMessage(
              "heelo this is another a very long help string that I am trying "
              "to write to intentionally make it long for the sake of things.");
  feta::detail::Argument arg3 =
      feta::get_blank_argument()
          .withKey("-c")
          .withOptional(false)
          .withHelpMessage(
              "heelo this is another another a very long help string that I am "
              "trying to write to intentionally make it long for the sake of "
              "things.");
  feta::detail::Argument arg4 =
      feta::get_blank_argument()
          .withKey("-d")
          .withOptional(false)
          .withHelpMessage(
              "heelo this is another another a very long help string that I am "
              "trying to write to intentionally make it long");
  argparser.add(&arg);
  argparser.add(&dep);
  argparser.add(&arg2, &dep);
  argparser.add(&arg3, &dep);
  argparser.add(&arg4, &dep);
  std::vector<std::string> strs = argparser.get_help_message("test_app", true);
  for (int i = 0; i < (int)strs.size(); i++) {
    std::printf("%s\n", strs[i].c_str());
  }
  ASSERT_TRUE(true);
}
