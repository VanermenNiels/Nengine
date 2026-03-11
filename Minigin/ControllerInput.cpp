#include "ControllerInput.h"

dae::ControllerInput::ControllerInput(unsigned int controllerIndex)
	: m_Index{controllerIndex}
{
	
}

void dae::ControllerInput::Update()
{
	m_LastState = m_State;

	// Retrieve current state from the controller
	ZeroMemory(&m_State, sizeof(XINPUT_STATE));
	XInputGetState(m_Index, &m_State);
}

bool dae::ControllerInput::IsConnected() const
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(state));
	return XInputGetState(m_Index, &state) == ERROR_SUCCESS;
}

bool dae::ControllerInput::IsPressed(WORD button) const
{
	return !(m_LastState.Gamepad.wButtons & button) &&
		   (m_State.Gamepad.wButtons & button);
}

bool dae::ControllerInput::IsReleased(WORD button) const
{
	return (m_State.Gamepad.wButtons & button) &&
		   !(m_LastState.Gamepad.wButtons & button);
}

bool dae::ControllerInput::IsDown(WORD button) const
{
	return (m_State.Gamepad.wButtons & button) != 0;
}
