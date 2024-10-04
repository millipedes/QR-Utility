#include "QRCode.h"

auto string_to_bitset(const std::string& input) -> AsciiBits {
  auto transform_view = input | std::views::transform([](auto c) {
      return std::bitset<8>(static_cast<unsigned char>(c));
  });
  // gcc doesn't suppot std::ranges::to yet, but I would use that here
  AsciiBits result;
  for (const auto& bits : transform_view) {
    result.push_back(bits);
  }
  return result;
}

QRCode qr_code_instance;

auto QRCode::draw_outline_square(const QRCodeCoord& upper_left,
    const int& side_len) -> void {
  for (int i = upper_left[1]; i < upper_left[1] + side_len; i++) {
    for (int j = upper_left[0]; j < upper_left[0] + side_len; j++) {
      if ((i == upper_left[1]
            || i == (upper_left[1] + side_len - 1)
            || j == upper_left[0]
            || j == (upper_left[0] + side_len - 1))
          && i < qr_side_len && i >= 0
          && j < qr_side_len && j >= 0) {
        id[i * qr_side_len + j] = true;
      }
    }
  }
}

template <std::size_t N>
auto QRCode::bits_to_rectangle(const QRCodeCoord& upper_left,
    const QRCodeCoord& bottom_right, const std::bitset<N>& bits) -> tl::expected<size_t, std::string> {
  auto width = bottom_right[0] - upper_left[0] + 1;
  auto height = bottom_right[1] - upper_left[1] + 1;
  auto qty_elements = width * height;
  if (qty_elements != N) {
    return tl::make_unexpected("bits_to_rectangle: number of bits: "
        + std::to_string(bits.size()) + " does not match the area of the "
        "rectangle: " + std::to_string(qty_elements));
  }
  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      auto index = (i * width) + j;
      if (!bits[index]) {
        id[(upper_left[1] + i) * (bottom_right[0] + 1) + upper_left[0] + j] = true;
      }
    }
  }
  return 0;
}

auto QRCode::draw_zebra_crossing() -> void {
  for (size_t i = zebra_crossing_start; i < zebra_crossing_end; i++) {
    for (size_t j = zebra_crossing_start; j < zebra_crossing_end; j++) {
      if (((i == zebra_crossing_start && j % 2 == 1)
            || (j == zebra_crossing_start && i % 2 == 1))
          && !(i == zebra_crossing_start && j == zebra_crossing_start)) {
        id[i * qr_side_len + j] = true;
      }
    }
  }
}

auto QRCode::draw_type() -> void {
  std::bitset<4> bits;
  switch (type) {
    case DataType::Numeric:
      bits = std::bitset<4>(static_cast<int>(DataType::Numeric));
      break;
    case DataType::AlphaNumeric:
      bits = std::bitset<4>(static_cast<int>(DataType::AlphaNumeric));
      break;
    case DataType::Binary:
      bits = std::bitset<4>(static_cast<int>(DataType::Binary));
      break;
    case DataType::JapaneseKanji:
      bits = std::bitset<4>(static_cast<int>(DataType::JapaneseKanji));
      break;
  }
  auto maybe_success = bits_to_rectangle(QRCodeCoord{23, 23}, QRCodeCoord{24, 24}, bits);
  if (!maybe_success) {
    throw std::runtime_error("draw_type: encounterd unrecoverable error: "
        + maybe_success.error());
  }
}

auto QRCode::draw_length() -> void {
  if (to_encode.size() >= std::pow(2, 8)) {
    throw std::runtime_error("draw_length: unrecoverable error: cannot encode "
        "strings 2^8 characters or longer");
  }
  auto bits = std::bitset<8>(to_encode.size());
  auto maybe_success = bits_to_rectangle(QRCodeCoord{23, 19}, QRCodeCoord{24, 22}, bits);
  if (!maybe_success) {
    throw std::runtime_error("draw_length: encounterd unrecoverable error: "
        + maybe_success.error());
  }
}

auto QRCode::draw_version_two_presets() -> void {
  // Upper Left Square
  draw_outline_square(QRCodeCoord{1, 1}, 5);
  draw_outline_square(QRCodeCoord{-1, -1}, 9);
  // Upper Right Square
  draw_outline_square(QRCodeCoord{17, -1}, 9);
  draw_outline_square(QRCodeCoord{qr_side_len - 6, 1}, 5);
  // Bottom Left Square
  draw_outline_square(QRCodeCoord{-1, 17}, 9);
  draw_outline_square(QRCodeCoord{1, 19}, 5);
  // Bottom Right Square
  draw_outline_square(QRCodeCoord{17, 17}, 3);
  draw_zebra_crossing();
  draw_type();
  draw_length();
}

auto QRCode::draw_background() -> void {
  auto window_pos = ImGui::GetWindowPos();
  auto window_size = ImGui::GetWindowSize();
  auto rect_min = ImVec2(
      window_pos.x + (window_size.x - default_sub_square_size) / 4 - default_sub_square_size,
      window_pos.y + (window_size.y - default_sub_square_size) / 4 - default_sub_square_size
  );
  auto rect_max = ImVec2(
      (qr_side_len + 2) * default_sub_square_size + rect_min.x,
      (qr_side_len + 2) * default_sub_square_size + rect_min.y
  );

  auto draw_list = ImGui::GetWindowDrawList();
  draw_list->AddRectFilled(rect_min, rect_max, imcolor_white);
}

auto QRCode::draw_qr_code() -> void {
  draw_background();

  size_t i_index = 0;
  size_t j_index = 0;
  auto window_pos = ImGui::GetWindowPos();
  auto window_size = ImGui::GetWindowSize();
  for (const auto& sub_square : qr_code_instance.id) {

    auto i_offset = default_sub_square_size * i_index;
    auto j_offset = default_sub_square_size * j_index;

    auto rect_min = ImVec2(
        j_offset + window_pos.x + (window_size.x - default_sub_square_size) / 4,
        i_offset + window_pos.y + (window_size.y - default_sub_square_size) / 4
    );
    auto rect_max = ImVec2(
        rect_min.x + default_sub_square_size,
        rect_min.y + default_sub_square_size
    );

    auto draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(rect_min, rect_max,
        sub_square ? imcolor_white : imcolor_black);
    j_index++;
    if (j_index >= qr_side_len) {
      j_index = 0;
      i_index++;
    }
  }
}
