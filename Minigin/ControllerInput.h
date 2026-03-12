#pragma once
#include <windows.h>
#include <Xinput.h>

namespace dae
{
	class ControllerInput final
	{
    public:
        ControllerInput(unsigned int controllerIndex);

        ControllerInput(const ControllerInput&) = default;
        ControllerInput& operator=(const ControllerInput&) = default;

        ControllerInput(ControllerInput&&) noexcept = default;
        ControllerInput& operator=(ControllerInput&&) noexcept = default;

        void Update();

        bool IsConnected() const;

        // Button checks
        bool IsPressed(WORD button) const;
        bool IsReleased(WORD button) const;
        bool IsDown(WORD button) const;

    private:
        unsigned int m_Index;
        XINPUT_STATE m_State{};
        XINPUT_STATE m_LastState{};
	};
}