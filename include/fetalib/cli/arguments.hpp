#pragma once

#include <string>
#include <vector>

#include "fetalib/cli/fetalib_export.hpp"
#include "fetalib/common/util.hpp"

namespace feta
{

struct FETALIB_EXPORT Argument
{
  std::string key;
  std::string alternate_key;
  std::string help_message;
  bool is_flag;
  bool is_optional;
  int word_count;
};

class FETALIB_EXPORT ArgumentParser
{
public:
  ArgumentParser(int argc, char** argv)
      : argc {argc}
  {
    for (int i = 0; i < argc; i++) {
      this->argv.push_back(argv[i]);
    }
  }

  void add_option(std::string key,
                  std::string key_alternate = "",
                  std::string help_message = "",
                  bool is_flag = false,
                  int word_count = 1);
  void add_required(std::string key,
                    std::string key_alternate = "",
                    std::string help_message = "",
                    bool is_flag = false,
                    int word_count = 1);

  int get_argc() { return argc; };
  std::vector<std::string>* get_argv() { return &argv; }

  bool arg_exists(std::string key);
  Argument get_arg(std::string key);

  template<typename T>
  T get(std::string key)
  {
    throw std::invalid_argument(
        "unknown return value for fetalib::ArgumentParser::get_val.");
  }
  template<>
  std::vector<std::string> get<std::vector<std::string>>(std::string key)
  {
    Argument arg = get_arg(key);

    for (int i = 0; i < argv.size(); i++) {
      if ((std::string(argv[i]) == std::string(arg.key))
          || (std::string(argv[i]) == std::string(arg.alternate_key)))
      {
        if (arg.word_count == -1) {
          return util::vec_substring(argv, i + 1);
        } else {
          if (i + 1 + arg.word_count > argv.size()) {
            throw std::invalid_argument("no value given for argument.");
          }
          return util::vec_substring(argv, i + 1, i + 1 + arg.word_count);
        }
      }
    }

    throw std::invalid_argument("no value given for argument.");
  }
  template<>
  std::string get<std::string>(std::string key)
  {
    return util::join(get<std::vector<std::string>>(key), " ");
  }
  template<>
  int get<int>(std::string key)
  {
    return std::stoi(get<std::string>(key));
  }
  template<>
  bool get<bool>(std::string key)
  {
    Argument arg = get_arg(key);

    for (int i = 0; i < argv.size(); i++) {
      if ((std::string(argv[i]) == std::string(arg.key))
          || (std::string(argv[i]) == std::string(arg.alternate_key)))
      {
        if (arg.is_flag)
          return true;
        if (i + 1 >= argv.size()) {
          throw std::invalid_argument("no value given for argument.");
        }
        return argv[i + 1] == std::string("true");
      }
    }

    throw std::invalid_argument("no value given for argument.");
  }

private:
  int argc;
  std::vector<std::string> argv;
  std::vector<Argument> args;
};

}  // namespace feta
