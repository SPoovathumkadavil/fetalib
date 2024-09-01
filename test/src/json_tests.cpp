
#include <fetalib/json.hpp>
#include <gtest/gtest.h>
#include <map>

TEST(JsonTest, EnsureReadJsonKeys)
{
  if (!std::filesystem::exists("test/resources/test.json"))
    GTEST_SKIP() << "skipping test due to non-existant file";
  feta::JsonReader reader("test/resources/test.json");
  EXPECT_EQ(reader.get_keys()[0], std::string("key1"));
}

TEST(JsonTest, EnsureReadJsonEntry)
{
  if (!std::filesystem::exists("test/resources/test.json"))
    GTEST_SKIP() << "skipping test due to non-existant file";
  feta::JsonReader reader("test/resources/test.json");
  EXPECT_EQ(reader.read_l1_entry("key1"), std::string("hehe"));
}
