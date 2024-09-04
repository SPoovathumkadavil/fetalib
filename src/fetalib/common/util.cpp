
#include <numeric>
#include <sstream>

#include "fetalib/common/util.hpp"

namespace feta
{
// needs test
template<typename T>
std::vector<T> vec_substring(std::vector<T> vec, int a, int b)
{
  std::vector<T> ret;
  for (int c = a; c < b; c++) {
    if (c + 1 < b)
      ret.push_back(vec[c]);
  }
  return ret;
}

std::string& implode(const std::vector<std::string>& strings,
                     std::string delim,
                     std::string& s)
{
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

std::string join(const std::vector<std::string>& strings, std::string delim)
{
  std::string s;
  return implode(strings, delim, s);
}

template<typename T>
T convert_to(const std::string& str)
{
  std::istringstream ss(str);
  T num;
  ss >> num;
  return num;
}

}  // namespace feta