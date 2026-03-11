#pragma once
#include <windows.h>
#include <Xinput.h>
#include <array>

namespace dae
{
	class ControllerInput final
	{
    public:
        ControllerInput(unsigned int controllerIndex);

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