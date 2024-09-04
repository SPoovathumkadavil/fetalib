#pragma once

#include <string>
#include <vector>

namespace feta
{

template<typename T>
T convert_to(const std::string& str);

template<typename T>
std::vector<T> vec_substring(std::vector<T> vec, int a, int b);

std::string join(const std::vector<std::string>& strings, std::string delim);
std::string& implode(const std::vector<std::string>& strings, std::string delim, std::string& s);

}  // namespace feta