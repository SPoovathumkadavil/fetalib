
#include <type_traits>
#include <stdexcept>

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

template<typename T>
T ArgumentParser::get(std::string key)
{
  Argument* arg = get_arg(key);
  if (arg == nullptr)
    throw std::invalid_argument("argument does not exist.");

  for (int i = 0; i < _argv.size(); i++) {
    if (_argv[i] == arg->_key || _argv[i] == arg->_key_alternate) {
      // flag boolean return
      if (arg->_is_flag) {
        if (!std::is_same<T, bool>::value) {
          throw std::invalid_argument("flags represent boolean values.");
        } else {
          return true;
        }
      }

      // regular value return
      // "word count" defines approximately how many words to take for the
      // value of the argument. if = -1, then take the rest of the argv as
      // values.
      if (i + 1 + (arg->_word_count) < _argc) {
        if (arg->_word_count == -1) {
          return convert_to<T>(
              join(vec_substring(_argv, i + 1, _argv.size()), " "));
        } else {
          return convert_to<T>(
              join(vec_substring(_argv, i + 1, i + 1 + arg->_word_count), " "));
        }
      } else {
        throw std::invalid_argument("no value given for argument.");
      }
    }
  }

  // flag false return
  if (arg->_is_flag) {
    if (!std::is_same<T, bool>::value) {
      throw std::invalid_argument("flags represent boolean values.");
    } else {
      return false;
    }
  }

  // should never get here.
  throw std::invalid_argument("unexpected error.");
}

}  // namespace feta
