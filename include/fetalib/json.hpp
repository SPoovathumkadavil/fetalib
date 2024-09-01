#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>

#include "fetalib/fetalib_export.hpp"

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

}
