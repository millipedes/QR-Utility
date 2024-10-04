#pragma once

#include <cstdint>
#include <filesystem>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#include <tl/expected.hpp>

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace fs = std::filesystem;

constexpr static auto default_font_size = 16.0;
// personally I develop on 4k monitors, to be super accurate it would be a 1.92
// dpi scale factor but close enough
constexpr static auto default_dpi_factor = 2.0;

constexpr static auto default_font_path = "../imgui/misc/fonts/DroidSans.ttf";

constexpr static auto default_bg_color
    = std::array<double, 4>{0.0, 35.0 / 255.0, 66.0 / 255.0, 1.0};

auto init_imgui_context() -> ImGuiIO&;
auto start_imgui_frame() -> void;
auto setup_fonts(const uint32_t& dpi_factor, const fs::path& font_path,
    const ImGuiIO& io) -> tl::expected<uint32_t, std::string>;
auto imgui_render(SDL_Window* window, const ImGuiIO& io) -> void;
auto set_window_presets() -> void;
auto imgui_set_style() -> void;
