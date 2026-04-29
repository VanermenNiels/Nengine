#pragma once
#include "Observers/Observer.h"
#include <string>
#include <unordered_map>

namespace dae
{
    class ISoundService;

    class SoundObserver final : public Observer
    {
    public:
        SoundObserver(ISoundService* soundService);
        void BindSound(EventId eventId, const std::string& filename, int volume = 128);
        void onNotify(GameObject* entity, Event event) override;

    private:
        ISoundService* m_pSoundService;
        std::unordered_map<EventId, std::pair<std::string, int>> m_Bindings;
    };
}