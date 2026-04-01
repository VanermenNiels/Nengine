#pragma once
#include "Singleton.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
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

		void BindKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command, InputType inputType);
		void UnBindKeyboardCommand(SDL_Keycode key);

		void BindControllerCommand(int controllerIndex, WORD button, std::unique_ptr<Command> command, InputType inputType);

	private:

		struct InputBinding
		{
			InputType type;
			std::unique_ptr<Command> command;
		};

		std::unordered_map<SDL_Keycode, InputBinding> m_KeyBindings;

		std::vector<ControllerInput> m_Controllers;

		static const int MAX_CONTROLLERS{ 4 };

		std::array<std::map<WORD, InputBinding>, MAX_CONTROLLERS> m_ControllerBindings;

		// In the private section
		std::vector<bool> m_PreviousKeyboardState;
		static const int SDL_NUM_SCANCODES_COUNT{ 512 }; // SDL3's scancode count
	};
}
