#pragma once
#include "ISoundService.h"
#include <memory>

namespace dae
{
    class SDLMixerSoundService final : public ISoundService
    {
    public:
        SDLMixerSoundService();
        ~SDLMixerSoundService() override;

        SDLMixerSoundService(const SDLMixerSoundService&) = delete;
        SDLMixerSoundService& operator=(const SDLMixerSoundService&) = delete;

        void Play(const std::string& filename, int volume) override;
        void SetMasterVolume(int volume) override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
}