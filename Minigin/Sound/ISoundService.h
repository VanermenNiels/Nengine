#pragma once
#include <string>

namespace dae
{
    class ISoundService
    {
    public:
        virtual ~ISoundService() = default;
        virtual void Play(const std::string& filename, int volume) = 0;
        virtual void SetMasterVolume(int volume) = 0;
    };
}