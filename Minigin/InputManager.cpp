#include <backends/imgui_impl_sdl3.h>
#include <SDL3/SDL.h>
#include "InputManager.h"
#include <set>

void dae::InputManager::InitializeControllers()
{
#ifndef __EMSCRIPTEN__
    m_Controllers.clear();
    m_Controllers.reserve(MAX_CONTROLLERS);
    for (int c{}; c < MAX_CONTROLLERS; ++c)
        m_Controllers.emplace_back(c);
#endif
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

    int numKeys{};
    const bool* keyboardState = SDL_GetKeyboardState(&numKeys);

    if (m_PreviousKeyboardState.empty())
        m_PreviousKeyboardState.assign(numKeys, false);

    std::set<int> firedGroups{};

    for (const auto& [key, inputBinding] : m_KeyBindings)
    {
        SDL_Scancode scancode = SDL_GetScancodeFromKey(key, SDL_KMOD_NONE);
        if (scancode == SDL_SCANCODE_UNKNOWN || scancode >= numKeys) continue;

        if (!inputBinding.ignoreExclusiveGroup &&
            inputBinding.exclusiveGroup >= 0 &&
            firedGroups.contains(inputBinding.exclusiveGroup))
            continue;

        const bool isDown{ keyboardState[scancode] };
        const bool wasDown{ m_PreviousKeyboardState[scancode] };

        bool shouldFire = false;
        switch (inputBinding.type)
        {
        case InputType::Pressed:  shouldFire = isDown && !wasDown;  break;
        case InputType::Released: shouldFire = !isDown && wasDown;  break;
        case InputType::Down:     shouldFire = isDown;              break;
        }

        if (shouldFire)
        {
            inputBinding.command->Execute(deltaTime);
            if (!inputBinding.ignoreExclusiveGroup && inputBinding.exclusiveGroup >= 0)
                firedGroups.insert(inputBinding.exclusiveGroup);
        }
    }

    std::copy(keyboardState, keyboardState + numKeys, m_PreviousKeyboardState.begin());

#ifndef __EMSCRIPTEN__
    for (int c{}; c < static_cast<int>(m_Controllers.size()); ++c)
    {
        auto& controller = m_Controllers[c];
        controller.Update();

        if (!controller.IsConnected())
            continue;

        std::set<int> controllerFiredGroups{};

        for (const auto& [button, inputBinding] : m_ControllerBindings[c])
        {
            if (!inputBinding.ignoreExclusiveGroup &&
                inputBinding.exclusiveGroup >= 0 &&
                controllerFiredGroups.contains(inputBinding.exclusiveGroup))
                continue;

            bool shouldFire = false;
            switch (inputBinding.type)
            {
            case InputType::Pressed:  shouldFire = controller.IsPressed(button);  break;
            case InputType::Released: shouldFire = controller.IsReleased(button); break;
            case InputType::Down:     shouldFire = controller.IsDown(button);     break;
            }

            if (shouldFire)
            {
                inputBinding.command->Execute(deltaTime);
                if (!inputBinding.ignoreExclusiveGroup && inputBinding.exclusiveGroup >= 0)
                    controllerFiredGroups.insert(inputBinding.exclusiveGroup);
            }
        }
    }
#endif
    return true;
}

void dae::InputManager::BindKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command, InputType inputType, int exclusiveGroup, bool ignoreExclusiveGroup)
{
    m_KeyBindings.emplace(key, InputBinding{ inputType, std::move(command), exclusiveGroup, ignoreExclusiveGroup });
}

void dae::InputManager::UnBindKeyboardCommand(SDL_Keycode key)
{
    m_KeyBindings.erase(key);
}

void dae::InputManager::ClearAllCommands()
{
    /*m_KeyBindings.clear();
    for (auto& binding : m_ControllerBindings)
        binding.clear();*/
}

#ifndef __EMSCRIPTEN__
void dae::InputManager::BindControllerCommand(int controllerIndex, WORD button, std::unique_ptr<Command> command, InputType inputType, int exclusiveGroup, bool ignoreExclusiveGroup)
{
    if (controllerIndex < 0 || controllerIndex >= static_cast<int>(m_ControllerBindings.size()))
        return;

    m_ControllerBindings[controllerIndex].emplace(button, InputBinding{ inputType, std::move(command), exclusiveGroup, ignoreExclusiveGroup });
}
#endif