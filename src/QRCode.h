#pragma once

#include <bitset>
#include <cmath>
#include <functional>
#include <ranges>

#include "imgui.h"

using AsciiBits = std::vector<std::bitset<8>>;

auto string_to_bitset(const std::string& input) -> AsciiBits;

constexpr static auto qr_width = 25;
constexpr static auto qty_sub_squares = static_cast<int>(std::pow(qr_width, 2));

constexpr static auto imcolor_white = ImColor(255, 255, 255, 255);
constexpr static auto imcolor_black = ImColor(0,     0,   0, 255);

constexpr static auto default_sub_square_size = 10;

struct QRCode {
  // true:  white color
  // false: black color
  // latin id -> itself
  std::vector<bool> id = std::vector<bool>(qty_sub_squares);
  auto draw_qr_code() -> void;
};

extern QRCode qr_code_instance;
