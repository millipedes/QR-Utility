#pragma once

#include <bitset>
#include <cmath>
#include <functional>
#include <tl/expected.hpp>
#include <ranges>

#include "imgui.h"

namespace V2QRCode {

using AsciiBits = std::vector<std::bitset<8>>;

auto string_to_bitset(const std::string& input) -> AsciiBits;

constexpr static auto qr_side_len = 25;
constexpr static auto qty_sub_squares = static_cast<int>(std::pow(qr_side_len, 2));

constexpr static auto imcolor_white = ImColor(255, 255, 255, 255);
constexpr static auto imcolor_black = ImColor(0,     0,   0, 255);

constexpr static auto default_sub_square_size = 20;

constexpr static auto zebra_crossing_start = 6;
constexpr static auto zebra_crossing_end = 17;

using QRCodeCoord = std::array<int, 2>;

enum class DataType {
  Numeric = 1,
  AlphaNumeric = 2,
  Binary = 4,
  JapaneseKanji = 8,
};

struct QRCode {
  // true:  white color
  // false: black color
  // latin id -> itself
  std::vector<bool> id = std::vector<bool>(qty_sub_squares);
  DataType type;
  std::string to_encode;

  auto draw_outline_square(const QRCodeCoord& upper_left,
      const int& side_len) -> void;
  template <std::size_t N>
  auto bits_to_rectangle(const QRCodeCoord& upper_left, const QRCodeCoord& bottom_right,
      const std::bitset<N>& bits) -> tl::expected<size_t, std::string>;

  auto draw_zebra_crossing() -> void;
  auto draw_type() -> void;
  auto draw_length() -> void;
  auto draw_to_encode() -> void;
  auto draw_version_two_presets() -> void;
  auto draw_background() -> void;
  auto draw_qr_code() -> void;
};

extern QRCode qr_code_instance;

} // namespace V2QRCode
