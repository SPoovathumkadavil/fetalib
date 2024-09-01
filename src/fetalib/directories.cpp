
#include <filesystem>

#include "fetalib/directories.hpp"

#include <pwd.h>
#include <unistd.h>
#include <algorithm>

#include "fetalib/json.hpp"

namespace feta
{

/*
 * The needed proj dirs:
 *  - library_dir (test vs not_test)
 *  - config_dir (test vs not_test)
 */

void directories::load_values()
{
  /* populate loc struct */

  if (!__test) {
    JsonReader loc_reader(__loc_file_location);

    std::vector<std::string> tempkeys = {
      "bin", "library", "config", "scripts", "workspace"
    };

    // read from json
    for (std::string key : loc_reader.get_keys()) {
      std::string val = loc_reader.read_l1_entry(key);
      if (key == "bin")
        __loc_dirs.bin = val;
      else if (key == "library")
        __loc_dirs.lib = val;
      else if (key == "config")
        __loc_dirs.conf = val;
      else if (key == "scripts")
        __loc_dirs.scripts = val;
      else if (key == "workspace")
        __loc_dirs.workspace = val;
      else
        continue;
      tempkeys.erase(std::find(tempkeys.begin(), tempkeys.end(), key));
    }

    // validate 
    if (tempkeys.size() > 0)
      throw std::invalid_argument(".loc.json does not contain a mandatory key.");
    
    // project dirs
    __proj_dirs.lib = __loc_dirs.lib / __project_name;
    __proj_dirs.conf = __loc_dirs.conf / __project_name;
  } else {
    // "test"/devel dirs
    __proj_dirs.lib = "library";
    __proj_dirs.conf = "config";
  }
}

directories::directories(std::string project_name)
    : __project_name {project_name}
    , __loc_file_location {std::string(get_home_dir().c_str()) + "/.loc.json"}
    , __test {false}
{
  load_values();
}

directories::directories(std::string project_name,
                         std::filesystem::path loc_file_location)
    : __project_name {project_name}
    , __loc_file_location {loc_file_location}
    , __test {false}
{
  load_values();
}

directories::directories(std::string project_name,
                         std::filesystem::path loc_file_location,
                         bool test)
    : __project_name {project_name}
    , __loc_file_location {loc_file_location}
    , __test {test}
{
  load_values();
}

std::filesystem::path directories::get_home_dir()
{
  const char* homedir;
  if ((homedir = getenv("HOME")) == NULL)
    homedir = getpwuid(getuid())->pw_dir;
  return std::filesystem::path(homedir);
}

std::filesystem::path directories::get_cwd()
{
  return std::filesystem::current_path();
}

std::string directories::project_name()
{
  return __project_name;
}

std::filesystem::path directories::loc_file_location()
{
  return __loc_file_location;
}

_loc_dirs directories::loc_dirs()
{
  return __loc_dirs;
}

_proj_dirs directories::proj_dirs()
{
  return __proj_dirs;
}

bool directories::is_test()
{
  return __test;
}

}  // namespace feta