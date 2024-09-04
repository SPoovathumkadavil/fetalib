#pragma once

#include <filesystem>
#include <map>
#include <string>

#include "fetalib/files/fetalib_export.hpp"

/**
 * A note about the MSVC warning C4251:
 * This warning should be suppressed for private data members of the project's
 * exported classes, because there are too many ways to work around it and all
 * involve some kind of trade-off (increased code complexity requiring more
 * developer time, writing boilerplate code, longer compile times), but those
 * solutions are very situational and solve things in slightly different ways,
 * depending on the requirements of the project.
 * That is to say, there is no general solution.
 *
 * What can be done instead is understand where issues could arise where this
 * warning is spotting a legitimate bug. I will give the general description of
 * this warning's cause and break it down to make it trivial to understand.
 *
 * C4251 is emitted when an exported class has a non-static data member of a
 * non-exported class type.
 *
 * The exported class in our case is the class below (exported_class), which
 * has a non-static data member (m_name) of a non-exported class type
 * (std::string).
 *
 * The rationale here is that the user of the exported class could attempt to
 * access (directly, or via an inline member function) a static data member or
 * a non-inline member function of the data member, resulting in a linker
 * error.
 * Inline member function above means member functions that are defined (not
 * declared) in the class definition.
 *
 * Since this exported class never makes these non-exported types available to
 * the user, we can safely ignore this warning. It's fine if there are
 * non-exported class types as private member variables, because they are only
 * accessed by the members of the exported class itself.
 *
 * The name() method below returns a pointer to the stored null-terminated
 * string as a fundamental type (char const), so this is safe to use anywhere.
 * The only downside is that you can have dangling pointers if the pointer
 * outlives the class instance which stored the string.
 *
 * Shared libraries are not easy, they need some discipline to get right, but
 * they also solve some other problems that make them worth the time invested.
 */

namespace feta
{

struct _loc_dirs
{
  std::filesystem::path bin;
  std::filesystem::path lib;
  std::filesystem::path conf;
  std::filesystem::path scripts;
  std::filesystem::path workspace;
};

/**
 * @brief interesting directories for development (install paths etc.)
 */
struct _proj_dirs
{
  std::filesystem::path lib;
  std::filesystem::path conf;
};

/**
 * @brief methods for interacting with the feta-framework directory structure.
 * (+utilities)
 */
class FETALIB_EXPORT directories
{
public:
  /**
   * @param name The name or your project.
   */
  directories(std::string project_name);
  /**
   * @param project_name The name of your project.
   * @param loc_file_location The absolute path to your .loc.json file
   */
  directories(std::string project_name,
              std::filesystem::path loc_file_location);
  /**
   * @param project_name The name of your project.
   * @param loc_file_location The absolute path to your .loc.json file
   * @param test Whether or not to use test values
   */
  directories(std::string project_name,
              std::filesystem::path loc_file_location,
              bool test);

  /**
   * @brief Returns the home directory of the user.
   * @return string path
   */
  static std::filesystem::path get_home_dir();
  /**
   * @brief Get current working directory, this is the directory where the
   * executable is run from.
   * @return string path
   */
  static std::filesystem::path get_cwd();
  /**
   * @brief Returns whether or not using test dir values.
   */
  bool is_test();
  /**
   * @brief Returns the provided loc file path
   */
  std::filesystem::path loc_file_location();
  /**
   * @brief Returns the provided string project name
   */
  std::string project_name();
  /**
   * @brief Returns the location map (read from .loc.json)
   */
  _loc_dirs loc_dirs();
  /**
   * @brief Returns the dir map (interesting paths for development)
   */
  _proj_dirs proj_dirs();

private:
  std::string __project_name;
  std::filesystem::path __loc_file_location;
  bool __test;
  _loc_dirs __loc_dirs;
  _proj_dirs __proj_dirs;
  void load_values();  // rename maybe
};

}  // namespace feta