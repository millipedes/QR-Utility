#include <stdexcept>

#include "IMGuiWrapper.h"
#include "SDLWrapper.h"

auto main(void) -> int {
  init_sdl();
  auto glsl_version = determine_glsl_version();
  auto maybe_window = create_window();
  if (!maybe_window) {
    throw std::runtime_error(maybe_window.error());
  }
  // It's not a copy for .value() to be called, but it get a little tedious
  // hence the assignment here.
  auto window = maybe_window.value();

  auto gl_context = setup_sdl_gl_context(window);

  auto io = init_imgui_context();
  ImGui::StyleColorsDark();

  setup_platform_renderers(window, gl_context, glsl_version);

  auto font_setup = setup_fonts(default_dpi_factor, default_font_path, io);
  if (!font_setup) {
    throw std::runtime_error(font_setup.error());
  }

  auto done = false;
  while (!done) {
    done = poll_events_sdl(window);
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10);
      continue;
    }

    start_imgui_frame();

    ImGui::Begin("Test Window 1");
    ImGui::Text("Hello IMGUI");
    ImGui::End();

    imgui_render(window, io);
  }

  cleanup_sdl(window, gl_context);

  return 0;
}
