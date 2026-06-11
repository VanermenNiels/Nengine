#pragma once

#ifndef __EMSCRIPTEN__
#include <windows.h>
#include <memory>

namespace dae
{
	class ControllerInput final
	{
    public:
        ControllerInput(unsigned int controllerIndex);

        ~ControllerInput();

        ControllerInput(const ControllerInput&) = default;
        ControllerInput& operator=(const ControllerInput&) = default;

        ControllerInput(ControllerInput&&) noexcept;
        ControllerInput& operator=(ControllerInput&&) noexcept;

        void Update();

        bool IsConnected() const;

        // Button checks
        bool IsPressed(WORD button) const;
        bool IsReleased(WORD button) const;
        bool IsDown(WORD button) const;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_ControllerInput;
	};
}

#endif