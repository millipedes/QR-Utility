cmake_minimum_required(VERSION 3.5)
project(qr_util)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(tl-expected REQUIRED)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(SRC_FILES
  ${SRC_DIR}/IMGuiWrapper.cpp
  ${SRC_DIR}/SDLWrapper.cpp
)

set(IMGUI_DIR imgui)
set(IMGUI_SOURCES
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(qr_util ${IMGUI_SOURCES} ${SRC_FILES} src/main.cpp)

target_include_directories(qr_util PRIVATE
  ${SRC_DIR}
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(qr_util PRIVATE
  glfw
  OpenGL::GL
  SDL2
  tl::expected
)

set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")
option(BUILD_TESTS "Build Test Suite" ON)
if(BUILD_TESTS)
  enable_testing()
  set(TEST_SRC_FILES)
  add_executable(tests ${TEST_SRC_FILES} test/main.cpp)
  target_link_libraries(tests
    gtest
    gtest_main
    pthread
    glfw
    OpenGL::GL
    SDL2
  )
endif()
