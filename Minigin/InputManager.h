#pragma once
#include "Singleton.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include "Commands/Command.h"
#include "ControllerInput.h"
#include <array>

namespace dae
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        enum class InputType
        {
            Pressed,
            Released,
            Down
        };

        void InitializeControllers();
        bool ProcessInput(float deltaTime);

        void BindKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command, InputType inputType, int exclusiveGroup = -1, bool ignoreExclusiveGroup = false);
        void UnBindKeyboardCommand(SDL_Keycode key);

        void BindControllerCommand(int controllerIndex, WORD button, std::unique_ptr<Command> command, InputType inputType, int exclusiveGroup = -1, bool ignoreExclusiveGroup = false);

    private:
        struct InputBinding
        {
            InputType type;
            std::unique_ptr<Command> command;
            int exclusiveGroup{ -1 };
            bool ignoreExclusiveGroup{ false };
        };

        std::unordered_multimap<SDL_Keycode, InputBinding> m_KeyBindings;

        std::vector<ControllerInput> m_Controllers;

        static const int MAX_CONTROLLERS{ 4 };

        std::array<std::multimap<WORD, InputBinding>, MAX_CONTROLLERS> m_ControllerBindings;

        std::vector<bool> m_PreviousKeyboardState;
    };
}