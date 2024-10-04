#include "SDLWrapper.h"

namespace Graphics {

auto init_sdl() -> void {
  if (SDL_Init(SDL_INIT_VIDEO
        | SDL_INIT_TIMER
        | SDL_INIT_GAMECONTROLLER) != 0) {
    throw std::runtime_error("SDL initialization error: " + std::string(SDL_GetError()));
  }
}

auto determine_glsl_version() -> std::string {
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  auto glsl_version = "#version 100";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  auto glsl_version = "#version 150";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  auto glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif
  return glsl_version;
}

auto create_window() -> tl::expected<SDL_Window*, std::string> {
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, sdl_double_buffer);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, sdl_depth_size);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, sdl_stencil_size);
  auto window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL
        | SDL_WINDOW_RESIZABLE
        | SDL_WINDOW_ALLOW_HIGHDPI);
  auto window = SDL_CreateWindow("QR Utility", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, window_width, window_height, window_flags);
  if (window == nullptr) {
    return tl::make_unexpected("SDL window creation error: "
        + std::string(SDL_GetError()));
  }
  return window;
}

auto setup_sdl_gl_context(SDL_Window* window) -> SDL_GLContext {
  auto gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync
  return gl_context;
}

auto setup_platform_renderers(SDL_Window* window, const SDL_GLContext& gl_context,
    const std::string& glsl_version) -> void {
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

auto poll_events_sdl(SDL_Window* window) -> bool {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) {
        return true;
    }
    if (event.type == SDL_WINDOWEVENT
        && event.window.event == SDL_WINDOWEVENT_CLOSE
        && event.window.windowID == SDL_GetWindowID(window)) {
        return true;
    }
  }
  return false;
}

auto cleanup_sdl(SDL_Window* window, const SDL_GLContext& gl_context) -> void {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

} // namespace Graphics
