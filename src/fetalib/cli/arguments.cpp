
#include "fetalib/cli/arguments.hpp"

void feta::ArgumentParser::add_option(std::string key,
                                      std::string alternate_key,
                                      std::string help_message,
                                      bool is_flag,
                                      int word_count)
{
  feta::Argument arg = {key, alternate_key, help_message, is_flag, true, word_count};
  args.push_back(arg);
}

void feta::ArgumentParser::add_required(std::string key,
                                        std::string alternate_key,
                                        std::string help_message,
                                        bool is_flag,
                                        int word_count)
{
  feta::Argument arg = {key, alternate_key, help_message, is_flag, false, word_count};
  args.push_back(arg);
}

bool feta::ArgumentParser::arg_exists(std::string key) {
  for (feta::Argument arg : args) {
    if (arg.key == key || arg.alternate_key == key) {
      return true;
    }
  }
  return false;
}

feta::Argument feta::ArgumentParser::get_arg(std::string key) {
  for (feta::Argument arg : args) {
    if (arg.key == key || arg.alternate_key == key) {
      return arg;
    }
  }
  throw std::invalid_argument("argument not found.");
}