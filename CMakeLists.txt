cmake_minimum_required(VERSION 3.26)
project(Nengine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EMSCRIPTEN)
    message("Using Emscripten!")
endif()

# -----------------------
# FetchContent
# -----------------------
include(FetchContent)

# -----------------------
# GLM (header-only, no linking needed)
# -----------------------
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
)
FetchContent_MakeAvailable(glm)

# -----------------------
# SDL3
# -----------------------
FetchContent_Declare(
    SDL3
    URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip
)
FetchContent_MakeAvailable(SDL3)
list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
find_package(SDL3 CONFIG REQUIRED)

# -----------------------
# SDL3_ttf
# -----------------------
FetchContent_Declare(
    SDL3_ttf
    URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip
)
FetchContent_MakeAvailable(SDL3_ttf)
list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
find_package(SDL3_ttf CONFIG REQUIRED)

# -----------------------
# SDL3_mixer (built from source)
# -----------------------
FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG        main
    GIT_SHALLOW    TRUE
)
set(SDL3MIXER_VENDORED ON)
set(SDL3MIXER_SAMPLES  OFF)
set(SDL3MIXER_INSTALL  OFF)
FetchContent_MakeAvailable(SDL3_mixer)

# -----------------------
# ImGui
# -----------------------
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.92.5-docking
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${sdl3_SOURCE_DIR}/include
)
target_link_libraries(imgui PUBLIC SDL3::SDL3)

# -----------------------
# Optional tools
# -----------------------
if(WIN32)
    find_package(VLD CONFIG)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/steam")
find_package(Steamworks QUIET)

# -----------------------
# Subdirectories
# -----------------------
add_subdirectory(Minigin)
add_subdirectory(Game)