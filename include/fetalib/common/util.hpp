#pragma once

#include <string>
#include <vector>

#include "fetalib/common/fetalib_export.hpp"

namespace feta
{

struct FETALIB_EXPORT Validation
{
  bool valid;
  std::string detail;
};

struct FETALIB_EXPORT util {

  static std::vector<std::string> vec_substring(std::vector<std::string> *vec, int a);
  static std::vector<std::string> vec_substring(std::vector<std::string> *vec, int a, int b);
  static std::string join(const std::vector<std::string> strings, std::string delim);
  static std::string implode(const std::vector<std::string> strings, std::string delim, std::string s);

};

}  // namespace feta