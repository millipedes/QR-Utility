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

auto QRCode::draw_qr_code() -> void {
  size_t i_index = 0;
  size_t j_index = 0;
  for (const auto& sub_square : qr_code_instance.id) {
    auto window_pos = ImGui::GetWindowPos();
    auto window_size = ImGui::GetWindowSize();

    auto i_offset = default_sub_square_size * i_index;
    auto j_offset = default_sub_square_size * j_index;

    auto rect_min = ImVec2(
        j_offset + window_pos.x + (window_size.x - default_sub_square_size) / 4,
        i_offset + window_pos.y + (window_size.y - default_sub_square_size) / 4
    );
    auto rect_max = ImVec2(
        j_offset + rect_min.x + default_sub_square_size,
        i_offset + rect_min.y + default_sub_square_size
    );

    auto draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(rect_min, rect_max,
        sub_square ? imcolor_white : imcolor_black);
    j_index++;
    if (j_index >= qr_width) {
      j_index = 0;
      i_index++;
    }
  }
}
