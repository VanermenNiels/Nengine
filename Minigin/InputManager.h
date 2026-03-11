#pragma once
#include "Singleton.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include "Commands/Command.h"
#include "ControllerInput.h"

namespace dae
{

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		bool ProcessInput();

		void BindKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command);
		void UnBindKeyboardCommand(SDL_Keycode key);

		
		void BindControllerCommand(unsigned int controllerIndex, WORD button, std::unique_ptr<Command> command);
		void UnBindControllerCommand(unsigned int controllerIndex, WORD button);

	private:
		std::unordered_map<SDL_Keycode, std::unique_ptr<Command>> m_KeyBindings;

		std::vector<ControllerInput> m_Controllers;
		std::vector<std::unordered_map<WORD, std::unique_ptr<Command>>> m_ControllerBindings;
	};
}
