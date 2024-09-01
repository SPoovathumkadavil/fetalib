#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>

namespace feta
{

class JsonReader
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
