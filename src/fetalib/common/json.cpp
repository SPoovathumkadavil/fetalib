
#include <fstream>

#include "fetalib/common/json.hpp"

namespace feta
{

JsonReader::JsonReader(std::string file_name)
    : _file_name {file_name}
{
}

std::string JsonReader::read_l1_entry(std::string key)
{
  std::ifstream file(_file_name);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      if (line.find(key) != std::string::npos) {
        std::string value = line.substr(line.find(":") + 1);
        // remove leading and trailing whitespaces and quotes
        value.erase(0, value.find_first_not_of(" \t\n\r\f\v\""));
        value.erase(value.find_last_of("\""));
        return value;
      }
    }
    file.close();
  }
  return "";
}

std::vector<std::string> JsonReader::get_keys()
{
  if (!std::filesystem::exists(_file_name))
    throw std::invalid_argument("recieved non-existant file path.");
  std::ifstream file(_file_name);
  std::vector<std::string> keys;
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      if (line.find_first_of("\"\'") != std::string::npos) {
        std::string value = line.substr(line.find_first_of("\"\'") + 1);
        value.erase(value.find_first_of("\"\'"));
        keys.push_back(value);
      }
    }
    file.close();
  }
  return keys;
}

std::string JsonReader::file_name()
{
  return _file_name;
}

}  // namespace feta