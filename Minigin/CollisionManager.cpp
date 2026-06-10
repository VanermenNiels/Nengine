#include "CollisionManager.h"
#include "HitboxComponent.h"

void dae::CollisionManager::RegisterHitbox(HitboxComponent* hitbox)
{
	m_Hitboxes.push_back(hitbox);
}

void dae::CollisionManager::UnregisterHitbox(HitboxComponent * hitbox)
{
	auto it { std::find(m_Hitboxes.begin(), m_Hitboxes.end(), hitbox) };
	if (it != m_Hitboxes.end())
		m_Hitboxes.erase(it);
}

void dae::CollisionManager::Update()
{
	for (auto* hb : m_Hitboxes)
	{
		hb->ClearFrameOverlaps();
	}

	if (m_Hitboxes.size() < 2) return;
	for (size_t h {}; h < m_Hitboxes.size() - 1; ++h)
	{
		auto hitbox { m_Hitboxes[h] };

		if (!hitbox->IsEnabled())
			continue;

		for (size_t o { h + 1}; o < m_Hitboxes.size(); ++o)
		{
			auto otherHitbox{ m_Hitboxes[o] };
			if (!otherHitbox->IsEnabled())
				continue;

			bool aCaresAboutB { (hitbox->GetTargetTags() & otherHitbox->GetOwnerTag()) != 0 };
			bool bCaresAboutA { (otherHitbox->GetTargetTags() & hitbox->GetOwnerTag()) != 0 };

			if (!aCaresAboutB && !bCaresAboutA) continue;

			if (hitbox->Overlaps(otherHitbox))
			{
				if (aCaresAboutB) hitbox->NotifyOverlapBegin(otherHitbox);
				if (bCaresAboutA) otherHitbox->NotifyOverlapBegin(hitbox);
			}

		}
	}
}
