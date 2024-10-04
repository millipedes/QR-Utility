#pragma once

#include <SDL.h>
#include <stdexcept>
#include <string>
#include <tl/expected.hpp>

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace Graphics {

static constexpr auto sdl_double_buffer = 1;
static constexpr auto sdl_depth_size = 24;
static constexpr auto sdl_stencil_size = 8;

static constexpr auto window_width = 1280;
static constexpr auto window_height = 720;

auto init_sdl() -> void;
auto determine_glsl_version() -> std::string;
auto create_window() -> tl::expected<SDL_Window*, std::string>;
auto setup_sdl_gl_context(SDL_Window* window) -> SDL_GLContext;
auto setup_platform_renderers(SDL_Window* window, const SDL_GLContext& gl_context,
    const std::string& glsl_version) -> void;
auto poll_events_sdl(SDL_Window* window) -> bool;
auto cleanup_sdl(SDL_Window* window, const SDL_GLContext& gl_context) -> void;

} // namespace Graphics
