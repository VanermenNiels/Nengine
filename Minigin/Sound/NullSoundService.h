#pragma once
#include "ISoundService.h"

namespace dae
{
    class NullSoundService final : public ISoundService
    {
    public:
        void Play(const std::string&, int) override {}
        void SetMasterVolume(int) override {}
    };
}