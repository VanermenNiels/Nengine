#pragma once
#include "BaseComponent.h"
#include "Events/Subject.h"
#include "Observers/Observer.h"
#include "Events/Event.h"
#include <vector>

namespace dae
{
	struct Rectf
	{
		float x, y, width, height;
	};

	class HitboxComponent final : public BaseComponent
	{
	public:
		HitboxComponent(GameObject* owner, Event beginOE, Event endOE, float width, float height, float offsetX = 0.f, float offsetY = 0.f);
		~HitboxComponent();

		Rectf GetWorldRect();
		bool  Overlaps(HitboxComponent* other);

		void SetTargetTags(uint32_t tags) { m_TargetTagMask = tags; }
		uint32_t GetTargetTags() const { return m_TargetTagMask; }

		void AddObserver(Observer* o) { m_Subject.AddObserver(o); }
		void RemoveObserver(Observer* o) { m_Subject.RemoveObserver(o); }

		void NotifyOverlapBegin(HitboxComponent* other);
		void NotifybeginOEnd(HitboxComponent* other);
		void ClearFrameOverlaps();

	private:
		float    m_Width, m_Height;
		float    m_OffsetX, m_OffsetY;
		uint32_t m_TargetTagMask{ 0xFFFFFFFF };

		Subject m_Subject{};

		Event m_BeginbeginOEvent;
		Event m_EndbeginOEvent;

		std::vector<GameObject*> m_CurrentOverlaps{};
		std::vector<GameObject*> m_PreviousOverlaps{};
	};
}