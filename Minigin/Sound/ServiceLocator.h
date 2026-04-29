#pragma once
#include "ISoundService.h"
#include "NullSoundService.h"

namespace dae
{
    class ServiceLocator final
    {
    public:
        static ISoundService& GetSoundService() { return *m_pSoundService; }

        static void RegisterSoundService(ISoundService* service)
        {
            m_pSoundService = service ? service : &m_NullService;
        }

    private:
        static ISoundService* m_pSoundService;
        static NullSoundService m_NullService;
    };
}