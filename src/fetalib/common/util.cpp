
#include <numeric>
#include <sstream>

#include "fetalib/common/util.hpp"

namespace feta
{

std::vector<std::string> util::vec_substring(std::vector<std::string> *vec, int a)
{
  return vec_substring(vec, a, vec->size());
}

std::vector<std::string> util::vec_substring(std::vector<std::string> *vec, int a, int b)
{
  std::vector<std::string> ret;
  for (int c = a; c < b; c++) {
    if (c + 1 <= b)
      ret.push_back(vec->at(c));
  }
  return ret;
}

std::string util::implode(const std::vector<std::string> strings,
                     std::string delim,
                     std::string s)
{
  if (strings.size() == 1) return strings[0];
  for (std::vector<std::string>::const_iterator str = strings.begin();
       str != strings.end();
       ++str)
  {
    s += (*str);
    if (str + 1 != strings.end()) {
      s += delim;
    }
  }

  return s;
}

std::string util::join(const std::vector<std::string> strings, std::string delim)
{
  std::string s;
  return implode(strings, delim, s);
}

}  // namespace feta