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
		void InitializeControllers();
		bool ProcessInput(float deltaTime);

		void BindKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command);
		void UnBindKeyboardCommand(SDL_Keycode key);

		void BindControllerCommand(int controllerIndex, WORD button, std::unique_ptr<Command> command);

	private:
		std::unordered_map<SDL_Keycode, std::unique_ptr<Command>> m_KeyBindings;

		std::vector<ControllerInput> m_Controllers;

		static const int MAX_CONTROLLERS{ 4 };

		std::array<std::map<WORD, std::unique_ptr<Command>>, MAX_CONTROLLERS> m_ControllerBindings;
	};
}
