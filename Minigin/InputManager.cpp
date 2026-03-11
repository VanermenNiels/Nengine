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
        if (e.type == SDL_EVENT_QUIT) {
            return false;
        }

        if (e.type == SDL_EVENT_KEY_DOWN) 
        {
            auto target { m_KeyBindings.find(e.key.key) };

            if (target != m_KeyBindings.end() && target->second)
                target->second->Execute();
        }

        ImGui_ImplSDL3_ProcessEvent(&e);
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


