#include "HitboxComponent.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include <algorithm>

dae::HitboxComponent::HitboxComponent(GameObject* owner, Event overlapEvent, float width, float height, float offsetX, float offsetY)
	: BaseComponent(owner)
	, m_Width{ width }
	, m_Height{ height }
	, m_OffsetX{ offsetX }
	, m_OffsetY{ offsetY }
	,m_OverlapEvent{ overlapEvent }
{
	CollisionManager::GetInstance().RegisterHitbox(this);
}

dae::HitboxComponent::~HitboxComponent()
{
	CollisionManager::GetInstance().UnregisterHitbox(this);
}

dae::Rectf dae::HitboxComponent::GetWorldRect()
{
	const auto& pos = GetOwner()->GetWorldPosition();
	return { pos.x + m_OffsetX, pos.y + m_OffsetY, m_Width, m_Height };
}

bool dae::HitboxComponent::Overlaps(HitboxComponent* other)
{
	auto thisRect { GetWorldRect() };
	auto otherRect { other->GetWorldRect() };

	return  thisRect.x < otherRect.x + otherRect.width &&
			thisRect.x + thisRect.width > otherRect.x &&
			thisRect.y < otherRect.y + otherRect.height &&
			thisRect.y + thisRect.height > otherRect.y;
}

void dae::HitboxComponent::NotifyOverlapBegin(HitboxComponent* other)
{
	m_CurrentOverlaps.push_back(other->GetOwner());

	bool wasOverlapping = std::find(m_PreviousOverlaps.begin(),
		m_PreviousOverlaps.end(), other->GetOwner())
		!= m_PreviousOverlaps.end();

	if (!wasOverlapping)
	{
		/*Event event{ make_sdbm_hash("OnOverlapBegin") };*/
		m_Subject.Notify(GetOwner(), m_OverlapEvent);
	}
}

void dae::HitboxComponent::NotifyOverlapEnd(HitboxComponent* other)
{
	Event event{ make_sdbm_hash("OnOverlapEnd") };
	m_Subject.Notify(other->GetOwner(), event);
}

void dae::HitboxComponent::ClearFrameOverlaps()
{
	for (auto* prev : m_PreviousOverlaps)
	{
		bool stillOverlapping { std::find(m_CurrentOverlaps.begin(), m_CurrentOverlaps.end(), prev) != m_CurrentOverlaps.end() };

		if (!stillOverlapping)
			NotifyOverlapEnd(prev->GetComponent<HitboxComponent>());
	}

	m_PreviousOverlaps = std::move(m_CurrentOverlaps);
	m_CurrentOverlaps.clear();
}