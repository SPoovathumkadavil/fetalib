#include <fetalib/directories.hpp>
#include <fetalib/json.hpp>
#include <gtest/gtest.h>
#include <map>
#include <filesystem>

TEST(DirTest, EnsureTestProjDirGetter)
{
  feta::directories fetadirs("test_project", "", true);
  auto pdir = fetadirs.proj_dirs();
  EXPECT_EQ(std::string(pdir.lib.c_str()), std::string("library"));
}

TEST(DirTest, EnsureNoTestThrowsWithIncorrectLoc)
{
  EXPECT_THROW({
    feta::directories fetadirs("test_project", "test/resources/test.json");
  }, std::invalid_argument);
}

TEST(DirTest, EnsureNoTestProjDirGetter)
{
  feta::directories fetadirs("test_project");
  feta::JsonReader reader(fetadirs.loc_file_location());
  auto pdir = fetadirs.proj_dirs();
  std::filesystem::path test_path = std::filesystem::path(reader.read_l1_entry("library")) / std::filesystem::path(fetadirs.project_name());
  EXPECT_EQ(pdir.lib, test_path);
}
