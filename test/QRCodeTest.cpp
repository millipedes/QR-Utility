#include <gtest/gtest.h>

#include "QRCode.h"

TEST(bitset, string_to_bitset_1) {
  auto input = "ABCD";
  std::vector<std::string> expected = {
      "01000001",
      "01000010",
      "01000011",
      "01000100"
  };
  auto string_as_bits = string_to_bitset(input);

  size_t index = 0;
  for (const auto& bits : string_as_bits) {
    EXPECT_EQ(bits.to_string(), expected[index++]);
  }
}
