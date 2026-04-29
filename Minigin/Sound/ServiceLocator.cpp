#include "ServiceLocator.h"

dae::NullSoundService dae::ServiceLocator::m_NullService{};
dae::ISoundService* dae::ServiceLocator::m_pSoundService = &dae::ServiceLocator::m_NullService;