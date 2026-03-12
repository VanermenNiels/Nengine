#include <backends/imgui_impl_sdl3.h>
#include <SDL3/SDL.h>
#include "InputManager.h"

void dae::InputManager::InitializeControllers()
{
    const int maxControllers = 4;

    m_Controllers.clear();
    m_Controllers.reserve(maxControllers);

    for (unsigned int i = 0; i < maxControllers; ++i)
    {
        m_Controllers.emplace_back(i);
    }

    // No need to resize m_ControllerBindings; std::array is fixed-size
}

bool dae::InputManager::ProcessInput(float deltaTime)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
            return false;

        ImGui_ImplSDL3_ProcessEvent(&e);
    }

    // --- Keyboard ---
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    for (auto& [key, command] : m_KeyBindings)
    {
        if (keyboardState[SDL_GetScancodeFromKey(key, SDL_KMOD_NONE)])
        {
            command->Execute(deltaTime);
        }
    }

    // --- Controllers ---
    for (int c = 0; c < static_cast<int>(m_Controllers.size()); ++c)
    {
        auto& controller = m_Controllers[c];
        controller.Update();

        if (!controller.IsConnected())
            continue;

        for (auto& [button, command] : m_ControllerBindings[c])
        {
            // Use IsDown() for continuous movement
            if (controller.IsDown(button))
            {
                command->Execute(deltaTime);
            }
        }
    }

    return true;
}

void dae::InputManager::BindKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command)
{
    m_KeyBindings.emplace(key, std::move(command));
}

void dae::InputManager::UnBindKeyboardCommand(SDL_Keycode key)
{
    m_KeyBindings.erase(key);
}

void dae::InputManager::BindControllerCommand(int controllerIndex, WORD button, std::unique_ptr<Command> command)
{
    if (controllerIndex < 0 || controllerIndex >= static_cast<int>(m_ControllerBindings.size()))
        return;

    m_ControllerBindings[controllerIndex].emplace(button, std::move(command));
}