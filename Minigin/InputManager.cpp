#include <backends/imgui_impl_sdl3.h>
#include <SDL3/SDL.h>
#include "InputManager.h"


dae::InputManager::InputManager()
{
    const int maxControllers = 4;

    m_Controllers.reserve(maxControllers);
    //m_ControllerBindings.resize(maxControllers);

    for (unsigned int i = 0; i < maxControllers; ++i)
        m_Controllers.emplace_back(i);
}

bool dae::InputManager::ProcessInput()
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
            return false;

        ImGui_ImplSDL3_ProcessEvent(&e);
    }

    const bool* keyboardState = SDL_GetKeyboardState(nullptr);

    for (auto& [key, command] : m_KeyBindings)
    {
        if (keyboardState[SDL_GetScancodeFromKey(key, SDL_KMOD_NONE)])
        {
            command->Execute();
        }
    }

    for (size_t i{}; i < m_Controllers.size(); ++i)
    {
        m_Controllers[i].Update();
    }

    return true;
}

void dae::InputManager::BindKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command)
{
    m_KeyBindings.emplace( key, std::move(command));

}

void dae::InputManager::UnBindKeyboardCommand(SDL_Keycode key)
{
    m_KeyBindings.erase(key);
}


