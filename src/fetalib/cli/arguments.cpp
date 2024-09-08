
#include "fetalib/cli/arguments.hpp"

void feta::ArgumentParser::add_option(std::string key,
                                      std::string alternate_key,
                                      std::string help_message,
                                      bool is_flag,
                                      int word_count)
{
  feta::detail::Argument arg = {
      key, alternate_key, help_message, is_flag, true, word_count};
  args.push_back(arg);
}

void feta::ArgumentParser::add_required(std::string key,
                                        std::string alternate_key,
                                        std::string help_message,
                                        bool is_flag,
                                        int word_count)
{
  feta::detail::Argument arg = {
      key, alternate_key, help_message, is_flag, false, word_count};
  args.push_back(arg);
}

bool feta::ArgumentParser::arg_exists(std::string key)
{
  for (feta::detail::Argument arg : args) {
    if (arg.key == key || arg.alternate_key == key) {
      return true;
    }
  }
  return false;
}

feta::detail::Argument feta::ArgumentParser::get_arg(std::string key)
{
  for (feta::detail::Argument arg : args) {
    if (arg.key == key || arg.alternate_key == key) {
      return arg;
    }
  }
  throw std::invalid_argument("argument not found.");
}

feta::Validation feta::ArgumentParser::validate()
{
  // ensure all required values accounted for.
  for (feta::detail::Argument arg : args) {
    if (!arg.is_optional) {
      bool found = false;
      for (std::string str : argv) {
        if (str.find(arg.key) != std::string::npos
            || str.find(arg.key) != std::string::npos)
        {
          found = true;
        }
      }
      if (!found)
        return Validation {false, "required argument not found."};
    }
  }

  // ensure non-flag arg not followed by arg (or vec-end) (no value provided)
  // ensure single definition of arguments
  bool last_arg = false;
  for (int i = 0; i < argv.size(); i++) {
    if (arg_exists(argv[i])) {
      if (last_arg) {
        return Validation {false, "argument not provided a value."};
      }
      feta::detail::Argument arg = get_arg(argv[i]);
      if (!arg.is_flag) {
        if (i == argv.size() - 1)
          return Validation {false, "argument not provided a value."};
        last_arg = true;
      }
    } else {
      last_arg = false;
    }
    for (int j = i + 1; j < argv.size(); j++) {  // >1 def check
      if (argv[i] == argv[j])
        return Validation
        {
          false, "argument defined more than once."
        };
    }
  }

  return Validation {true, ""};
}
