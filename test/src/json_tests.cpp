
#include <map>

#include <fetalib/common/json.hpp>
#include <gtest/gtest.h>

TEST(JsonTest, EnsureReadJsonKeys)
{
  if (!std::filesystem::exists("test/resources/test.json"))
    GTEST_SKIP() << "skipping test due to non-existent file";
  feta::JsonReader reader("test/resources/test.json");
  EXPECT_EQ(reader.get_keys()[0], std::string("key1"));
}

TEST(JsonTest, EnsureReadJsonEntry)
{
  if (!std::filesystem::exists("test/resources/test.json"))
    GTEST_SKIP() << "skipping test due to non-existent file";
  feta::JsonReader reader("test/resources/test.json");
  EXPECT_EQ(reader.read_l1_entry("key1"), std::string("hehe"));
}
