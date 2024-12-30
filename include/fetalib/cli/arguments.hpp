#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "fetalib/cli/fetalib_export.hpp"
#include "fetalib/common/util.hpp"

namespace feta
{

namespace detail
{

struct FETALIB_EXPORT ArgumentDependency
{
  std::string key;
  std::string help_message;
};

struct FETALIB_EXPORT Argument
{
  std::string key;
  std::string alternate_key;
  std::string help_message;
  bool is_flag;
  bool is_optional;
  int word_count;
  std::vector<ArgumentDependency> dependencies;
  Argument withKey(std::string key)
  {
    Argument a = *this;
    a.key = key;
    return a;
  };
  Argument withAlternateKey(std::string alt_key)
  {
    Argument a = *this;
    a.alternate_key = alt_key;
    return a;
  };
  Argument withHelpMessage(std::string message)
  {
    Argument a = *this;
    a.help_message = message;
    return a;
  };
  Argument withWordCount(int count)
  {
    Argument a = *this;
    a.word_count = count;
    return a;
  }
  Argument withFlag(bool is_flag = true)
  {
    Argument a = *this;
    a.is_flag = is_flag;
    return a;
  };
  Argument withOptional(bool is_opt = true)
  {
    Argument a = *this;
    a.is_optional = is_opt;
    return a;
  }
  Argument withDependency(ArgumentDependency dep)
  {
    Argument a = *this;
    a.dependencies.push_back(dep);
    return a;
  }
};

template<typename T>
struct FETALIB_EXPORT identity
{
  typedef T type;
};

}  // namespace detail

static detail::Argument get_blank_argument()
{
  return feta::detail::Argument {"", "", "", false, false, 1, std::vector<detail::ArgumentDependency>()};
}

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

  void add(detail::Argument arg);

  int get_argc() { return argc; };
  std::vector<std::string>* get_argv() { return &argv; }

  bool arg_exists(std::string key);
  detail::Argument get_arg(std::string key);

  template<typename T>
  std::optional<T> get(std::string key)
  {
    return get(get_arg(key), detail::identity<T>());
  }

  template<typename T>
  std::optional<T> get(detail::Argument arg)
  {
    return get(arg, detail::identity<T>());
  }

  Validation validate();
  std::vector<std::string> get_help_message(std::string app_name, bool should_align_levels = true);

private:
  int argc;
  std::vector<std::string> argv;
  std::vector<detail::Argument> args;

  bool dependency_check(std::vector<feta::detail::ArgumentDependency> deps);
  
  std::string extract_help_string(detail::Argument arg, int i_tab, int max_chars, int override_out = -1);
  std::string extract_help_string(detail::ArgumentDependency dep, int i_tab, int max_chars, int override_out = -1);
  int find_largest_hs_offset(std::vector<detail::Argument> args);

  std::optional<std::vector<std::string>> get(
      detail::Argument arg, detail::identity<std::vector<std::string>>)
  {
    for (int i = 0; i < get_argv()->size(); i++) {
      if ((std::string(get_argv()->at(i)) == std::string(arg.key))
          || (std::string(get_argv()->at(i)) == std::string(arg.alternate_key)))
      {
        if (arg.word_count == -1) {
          return util::vec_substring(get_argv(), i + 1);
        } else {
          if (i + 1 + arg.word_count > get_argv()->size()) {
            return std::nullopt;
          }
          return util::vec_substring(get_argv(), i + 1, i + 1 + arg.word_count);
        }
      }
    }

    throw std::invalid_argument("no value given for argument.");
  }
  std::optional<std::string> get(detail::Argument arg,
                                 detail::identity<std::string>)
  {
    if (auto ss = get(arg, detail::identity<std::vector<std::string>>()))
      return util::join(ss.value(), " ");
    else
      return std::nullopt;
  }
  std::optional<int> get(detail::Argument arg, detail::identity<int>)
  {
    if (auto s = get(arg, detail::identity<std::string>()))
      return std::stoi(s.value());
    else
      return std::nullopt;
  }
  std::optional<float> get(detail::Argument arg, detail::identity<float>)
  {
    if (auto s = get(arg, detail::identity<std::string>()))
      return std::stof(s.value());
    else
      return std::nullopt;
  }
  std::optional<bool> get(detail::Argument arg, detail::identity<bool>)
  {
    for (int i = 0; i < get_argv()->size(); i++) {
      if ((std::string(get_argv()->at(i)) == std::string(arg.key))
          || (std::string(get_argv()->at(i)) == std::string(arg.alternate_key)))
      {
        if (arg.is_flag)
          return true;
        if (i + 1 >= get_argv()->size()) {
          /* throw std::invalid_argument("no value given for argument."); */
          return std::nullopt;
        }
        return get_argv()->at(i + 1) == std::string("true");
      }
    }

    /* throw std::invalid_argument("no value given for argument."); */
    if (arg.is_flag)
      return false;
    return std::nullopt;
  }
};

}  // namespace feta
