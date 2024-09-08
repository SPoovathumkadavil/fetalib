#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "fetalib/cli/fetalib_export.hpp"
#include "fetalib/common/util.hpp"

namespace feta
{

namespace detail
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

template<typename T>
struct FETALIB_EXPORT identity
{
  typedef T type;
};

}  // namespace detail

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
  detail::Argument get_arg(std::string key);

  template<typename T>
  T get(std::string key)
  {
    return get(key, detail::identity<T>());
  }

  Validation validate();

private:
  int argc;
  std::vector<std::string> argv;
  std::vector<detail::Argument> args;

  template<typename T>
  T get(std::string key, detail::identity<T>)
  {
    throw std::invalid_argument(
        "unknown return value for fetalib::ArgumentParser::get.");
  }
  std::vector<std::string> get(std::string key,
                               detail::identity<std::vector<std::string>>)
  {
    detail::Argument arg = get_arg(key);
    for (int i = 0; i < get_argv()->size(); i++) {
      if ((std::string(get_argv()->at(i)) == std::string(arg.key))
          || (std::string(get_argv()->at(i)) == std::string(arg.alternate_key)))
      {
        if (arg.word_count == -1) {
          return util::vec_substring(get_argv(), i + 1);
        } else {
          if (i + 1 + arg.word_count > get_argv()->size()) {
            throw std::invalid_argument("no value given for argument.");
          }
          return util::vec_substring(get_argv(), i + 1, i + 1 + arg.word_count);
        }
      }
    }

    throw std::invalid_argument("no value given for argument.");
  }
  std::string get(std::string key, detail::identity<std::string>)
  {
    return util::join(get<std::vector<std::string>>(key), " ");
  }
  int get(std::string key, detail::identity<int>)
  {
    return std::stoi(get<std::string>(key));
  }
  float get(std::string key, detail::identity<float>)
  {
    return std::stof(get<std::string>(key));
  }
  bool get(std::string key, detail::identity<bool>)
  {
    detail::Argument arg = get_arg(key);
    for (int i = 0; i < get_argv()->size(); i++) {
      if ((std::string(get_argv()->at(i)) == std::string(arg.key))
          || (std::string(get_argv()->at(i)) == std::string(arg.alternate_key)))
      {
        if (arg.is_flag)
          return true;
        if (i + 1 >= get_argv()->size()) {
          throw std::invalid_argument("no value given for argument.");
        }
        return get_argv()->at(i + 1) == std::string("true");
      }
    }

    throw std::invalid_argument("no value given for argument.");
  }
};

}  // namespace feta
