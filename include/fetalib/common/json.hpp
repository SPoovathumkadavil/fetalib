#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "fetalib/common/fetalib_export.hpp"

namespace feta
{

class FETALIB_EXPORT JsonReader
{
public:
  JsonReader(std::string file_name);
  std::string read_l1_entry(std::string key);
  std::vector<std::string> get_keys();

  std::string file_name();

private:
  std::string _file_name;
};

}  // namespace feta
