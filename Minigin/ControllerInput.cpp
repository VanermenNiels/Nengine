#ifndef __EMSCRIPTEN__
#include "ControllerInput.h"
#include <Xinput.h>

struct dae::ControllerInput::Impl
{
	unsigned int index{};
	XINPUT_STATE state{};
	XINPUT_STATE lastState{};
};

dae::ControllerInput::ControllerInput(unsigned int controllerIndex)
	:m_ControllerInput{std::make_unique<Impl>()}
{
	m_ControllerInput->index = controllerIndex;
}

dae::ControllerInput::~ControllerInput() = default;

dae::ControllerInput::ControllerInput(ControllerInput&&) noexcept = default;
dae::ControllerInput& dae::ControllerInput::operator=(ControllerInput&&) noexcept = default;

void dae::ControllerInput::Update()
{
	m_ControllerInput->lastState = m_ControllerInput->state;

	// Retrieve current state from the controller
	ZeroMemory(&m_ControllerInput->state, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerInput->index, &m_ControllerInput->state);
}

bool dae::ControllerInput::IsConnected() const
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(state));
	return XInputGetState(m_ControllerInput->index, &state) == ERROR_SUCCESS;
}

bool dae::ControllerInput::IsPressed(WORD button) const
{
	return !(m_ControllerInput->lastState.Gamepad.wButtons & button) &&
		   (m_ControllerInput->state.Gamepad.wButtons & button);
}

bool dae::ControllerInput::IsReleased(WORD button) const
{
	return (m_ControllerInput->state.Gamepad.wButtons & button) &&
		   !(m_ControllerInput->lastState.Gamepad.wButtons & button);
}

bool dae::ControllerInput::IsDown(WORD button) const
{
	return (m_ControllerInput->state.Gamepad.wButtons & button) != 0;
}

#endif
