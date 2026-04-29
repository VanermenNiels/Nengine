#include "SoundObserver.h"
#include "Sound/ISoundService.h"    // <-- add this, full definition needed to call Play()
#include "Sound/ServiceLocator.h"

namespace dae
{
    SoundObserver::SoundObserver(ISoundService* soundService)
        : m_pSoundService(soundService) {}

    void SoundObserver::BindSound(EventId eventId, const std::string& filename, int volume)
    {
        m_Bindings[eventId] = { filename, volume };
    }

    void SoundObserver::onNotify(GameObject* /*entity*/, Event event)
    {
        auto it = m_Bindings.find(event.id);
        if (it != m_Bindings.end())
            m_pSoundService->Play(it->second.first, it->second.second);
    }
}