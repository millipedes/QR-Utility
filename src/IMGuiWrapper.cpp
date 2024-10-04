#include "IMGuiWrapper.h"

namespace Graphics {

auto init_imgui_context() -> ImGuiIO& {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  auto& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  return io;
}

auto start_imgui_frame() -> void {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

auto setup_fonts(const uint32_t& dpi_factor, const fs::path& font_path,
    const ImGuiIO& io) -> tl::expected<uint32_t, std::string> {
  io.Fonts->Clear();
  if (!fs::exists(font_path)) {
    return tl::make_unexpected("File not found: `" + font_path.string() + "`");
  }
  io.Fonts->AddFontFromFileTTF(default_font_path,
      default_font_size * default_dpi_factor);
  io.Fonts->Build();
  return 0;
}

auto imgui_render(SDL_Window* window, const ImGuiIO& io) -> void {
  ImGui::Render();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  glClearColor(default_bg_color[0], default_bg_color[1],
      default_bg_color[2], default_bg_color[3]);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window);
}

auto set_window_presets() -> void {
  ImGui::SetNextWindowSize(ImVec2(400, 300));
  ImGui::SetNextWindowPos(ImVec2(100, 100));
}

auto imgui_set_style() -> void {
  auto& style = ImGui::GetStyle();
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
}

} // Graphics
