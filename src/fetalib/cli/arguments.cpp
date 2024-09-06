
#include <stdexcept>
#include <type_traits>
#include <stdio.h>

#include "fetalib/cli/arguments.hpp"

#include "fetalib/common/util.hpp"

namespace feta
{

ArgumentParser::ArgumentParser(int argc, char** argv)
    : _argc {argc}
{
  for (int i = 0; i < argc; i++)
    _argv.push_back(std::string(argv[i]));
}

void ArgumentParser::add_option(std::string key,
                                std::string key_alternate,
                                std::string help_message,
                                bool is_flag,
                                int word_count)
{
  _args.push_back(
      Argument {key, key_alternate, help_message, false, is_flag, word_count});
}

void ArgumentParser::add_required(std::string key,
                                  std::string key_alternate,
                                  std::string help_message,
                                  bool is_flag,
                                  int word_count)
{
  _args.push_back(
      Argument {key, key_alternate, help_message, true, is_flag, word_count});
}

Argument* ArgumentParser::get_arg(std::string key)
{
  Argument* ret = nullptr;
  for (Argument arg : _args) {
    if (arg._key == key || arg._key_alternate == key)
      return ret = &arg;
  }
  return ret;
}

bool ArgumentParser::arg_exists(std::string key)
{
  bool e = false;
  for (Argument arg : _args)
    if (arg._key == key || arg._key_alternate == key)
      e = true;
  return e;
}

std::vector<std::string> ArgumentParser::get_vec_val(std::string key)
{
  Argument* arg = get_arg(key);
  if (arg == nullptr)  // ensure arg exists
    throw std::invalid_argument("argument does not exist.");

  for (int i = 0; i < _argv.size(); i++) {
    if ((std::string(_argv[i]) == std::string(arg->_key)) || (std::string(_argv[i]) == std::string(arg->_key_alternate))) {
      if (arg->_word_count == -1) {
        return util::vec_substring(_argv, i + 1);
      } else {
        if (i + 1 + arg->_word_count > _argv.size()) {
          throw std::invalid_argument("no value given for argument.");
        }
        return util::vec_substring(_argv, i + 1, i + 1 + arg->_word_count);
      }
    }
  }

  throw std::invalid_argument("no value given for argument.");
}

std::string ArgumentParser::get_str_val(std::string key)
{
  return util::join(get_vec_val(key), " ");
}

int ArgumentParser::get_int_val(std::string key)
{
  Argument* arg = get_arg(key);
  if (arg == nullptr)  // ensure arg exists
    throw std::invalid_argument("argument does not exist.");
  if (arg->_word_count != 1)
    throw std::invalid_argument(
        "integer arguments require a single word value.");
  return std::stoi(get_str_val(key));
}

float ArgumentParser::get_float_val(std::string key)
{
  Argument* arg = get_arg(key);
  if (arg == nullptr)  // ensure arg exists
    throw std::invalid_argument("argument does not exist.");
  if (arg->_word_count != 1)
    throw std::invalid_argument("float arguments require a single word value.");
  return std::stof(get_str_val(key));
}

bool ArgumentParser::get_bool_val(std::string key)
{
  Argument* arg = get_arg(key);
  if (arg == nullptr)  // ensure arg exists
    throw std::invalid_argument("argument does not exist.");
  if (arg->_is_flag)
    return true;
  if (arg->_word_count != 1)
    throw std::invalid_argument(
        "non-flag boolean arguments require a single word value.");
  return get_str_val(key) == "true";
}

}  // namespace feta
