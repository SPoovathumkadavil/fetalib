
#include <fetalib/json.hpp>
#include <gtest/gtest.h>
#include <map>

TEST(JsonTest, EnsureReadJsonKeys)
{
  feta::JsonReader reader("test/resources/test.json");
  ASSERT_EQ(reader.get_keys()[0], std::string("key1"));
}

TEST(JsonTest, EnsureReadJsonEntry)
{
  feta::JsonReader reader("test/resources/test.json");
  ASSERT_EQ(reader.read_l1_entry("key1"), std::string("hehe"));
}
