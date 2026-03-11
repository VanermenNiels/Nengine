cmake_minimum_required(VERSION 3.26)
project(Minigin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EMSCRIPTEN)
    message("Using Emscripten!")
endif()

if(EMSCRIPTEN)
    set(TARGET_NAME minigin_web)
    set(TARGET_PARAM )
else()
    set(TARGET_NAME minigin)
    set(TARGET_PARAM WIN32)
endif()

# -----------------------
# Executable
# -----------------------

add_executable(${TARGET_NAME} ${TARGET_PARAM}
    Minigin/Font.cpp
    Minigin/GameObject.cpp
    Minigin/InputManager.cpp
    Minigin/Main.cpp
    Minigin/Minigin.cpp
    Minigin/Renderer.cpp
    Minigin/ResourceManager.cpp
    Minigin/Scene.cpp
    Minigin/SceneManager.cpp
    Minigin/BaseComponent.cpp
    Minigin/TextComponent.cpp
    Minigin/Texture2D.cpp
    Minigin/Transform.cpp
    Minigin/FPSComponent.cpp
    Minigin/RenderComponent.cpp
    Minigin/RotateComponent.cpp
    Minigin/TrashTheCasheComponent.cpp
    Minigin/ControllerInput.cpp
    Minigin/Commands/Command.cpp
    Minigin/Commands/MoveCommand.cpp)

# -----------------------
# FetchContent
# -----------------------

include(FetchContent)

# -----------------------
# GLM
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
# Include directories
# -----------------------
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Data
    $<TARGET_FILE_DIR:${TARGET_NAME}>/Data
)

target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Minigin
    ${imgui_SOURCE_DIR}
)

# -----------------------
# Link libraries
# -----------------------

target_link_libraries(${TARGET_NAME} PUBLIC
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    glm::glm
    imgui
    ${VLD_LIBRARIES}
)

# -----------------------
# Copy DLLs after build
# -----------------------

if(WIN32)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3>
    $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
    $<TARGET_FILE_DIR:${TARGET_NAME}>
)
endif()

# -----------------------
# Compiler warnings
# -----------------------

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Werror)
endif()