#pragma once

#include <string>
#include <vector>

#include "fetalib/cli/fetalib_export.hpp"

namespace feta
{

struct FETALIB_EXPORT Argument
{
  std::string _key;
  std::string _key_alternate;
  std::string _help_message;
  bool _is_optional;
  bool _is_flag;
  int _word_count; // -1 means indefinite, 1 is default
};

class FETALIB_EXPORT ArgumentParser
{
public:
  ArgumentParser(int argc, char **argv) : _argc {argc} {
    for (int i = 0; i < argc; i++) _argv.push_back(std::string(argv[i]));
  };

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
  
  Argument *get_arg(std::string key);
  bool arg_exists(std::string key);

  template<typename T>
  T get(std::string key);

  template<typename T, int N>
  T get(std::string key);

  std::string get_help_string();

private:
  int _argc;
  std::vector<std::string> _argv;
  std::vector<Argument> _args;
};

}  // namespace feta
