#include <backends/imgui_impl_sdl3.h>
#include <SDL3/SDL.h>
#include "InputManager.h"

void dae::InputManager::InitializeControllers()
{
    m_Controllers.clear();
    m_Controllers.reserve(MAX_CONTROLLERS);

    for (int c{}; c < MAX_CONTROLLERS; ++c)
    {
        m_Controllers.emplace_back(c);
    }
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
    int numKeys{};
    const bool* keyboardState = SDL_GetKeyboardState(&numKeys);

    // Initialize previous state on first run
    if (m_PreviousKeyboardState.empty())
        m_PreviousKeyboardState.assign(numKeys, false);

    for (const auto& [key, inputBinding] : m_KeyBindings)
    {
        SDL_Scancode scancode = SDL_GetScancodeFromKey(key, SDL_KMOD_NONE);
        if (scancode == SDL_SCANCODE_UNKNOWN) continue;

        const bool isDown  { keyboardState[scancode] };
        const bool wasDown { m_PreviousKeyboardState[scancode] };

        switch (inputBinding.type)
        {
        case InputType::Pressed:
            if (isDown && !wasDown)
                inputBinding.command->Execute(deltaTime);
            break;

        case InputType::Released:
            if (!isDown && wasDown)
                inputBinding.command->Execute(deltaTime);
            break;

        case InputType::Down:
            if (isDown)
                inputBinding.command->Execute(deltaTime);
            break;
        }
    }

    // Save state for next frame — copy the raw array into the vector
    std::copy(keyboardState, keyboardState + numKeys, m_PreviousKeyboardState.begin());

    // --- Controllers ---
    for (int c{}; c < static_cast<int>(m_Controllers.size()); ++c)
    {
        auto& controller = m_Controllers[c];
        controller.Update();

        if (!controller.IsConnected())
            continue;

        for (const auto& [button, inputBinding] : m_ControllerBindings[c])
        {
            switch (inputBinding.type)
            {
            case InputType::Pressed:

                if (controller.IsPressed(button))
                    inputBinding.command->Execute(deltaTime);

				break;

             case InputType::Released:

                if (controller.IsReleased(button))
					inputBinding.command->Execute(deltaTime);

                break;

			 case InputType::Down:

                 if (controller.IsDown(button))
                     inputBinding.command->Execute(deltaTime);
                 break;
            }
        }
    }
    return true;
}

void dae::InputManager::BindKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command, InputType inputType)
{
    m_KeyBindings.emplace(key, InputBinding{ inputType, std::move(command) });
}

void dae::InputManager::UnBindKeyboardCommand(SDL_Keycode key)
{
    m_KeyBindings.erase(key);
}

void dae::InputManager::BindControllerCommand(int controllerIndex, WORD button, std::unique_ptr<Command> command, InputType inputType)
{
    if (controllerIndex < 0 || controllerIndex >= static_cast<int>(m_ControllerBindings.size()))
        return;

    m_ControllerBindings[controllerIndex].emplace(button, InputBinding{ inputType, std::move(command) });
}