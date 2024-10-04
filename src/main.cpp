#include <stdexcept>

#include "QRCode.h"
#include "IMGuiWrapper.h"
#include "SDLWrapper.h"

auto main(void) -> int {
  Graphics::init_sdl();
  auto glsl_version = Graphics::determine_glsl_version();
  auto maybe_window = Graphics::create_window();
  if (!maybe_window) {
    throw std::runtime_error(maybe_window.error());
  }
  // It's not a copy for .value() to be called, but it get a little tedious
  // hence the assignment here.
  auto window = maybe_window.value();

  auto gl_context = Graphics::setup_sdl_gl_context(window);

  auto io = Graphics::init_imgui_context();
  ImGui::StyleColorsDark();

  Graphics::setup_platform_renderers(window, gl_context, glsl_version);

  auto font_setup = Graphics::setup_fonts(Graphics::default_dpi_factor,
      Graphics::default_font_path, io);
  if (!font_setup) {
    throw std::runtime_error(font_setup.error());
  }

  Graphics::imgui_set_style();
  Graphics::set_window_presets();
  V2QRCode::qr_code_instance.type = V2QRCode::DataType::Binary;
  V2QRCode::qr_code_instance.to_encode = std::string("hello world");
  V2QRCode::qr_code_instance.QRCode::draw_version_two_presets();

  auto done = false;
  while (!done) {
    done = Graphics::poll_events_sdl(window);
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10);
      continue;
    }

    Graphics::start_imgui_frame();

    ImGui::Begin("Test Window 1");

    V2QRCode::qr_code_instance.draw_qr_code();

    ImGui::End();

    Graphics::imgui_render(window, io);
  }

  Graphics::cleanup_sdl(window, gl_context);

  return 0;
}
