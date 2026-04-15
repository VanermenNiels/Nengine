#include "ObserverComponent.h"

dae::ObserverComponent::ObserverComponent(GameObject* ownerRPtr, std::vector<EventId> reactsTo):
	BaseComponent(ownerRPtr),
	m_ReactsTo {std::move(reactsTo)}
{}

void dae::ObserverComponent::onNotify(GameObject*, Event event)
{
	if (m_ReactsTo.empty()) return;

	for (const auto& reactEvent : m_ReactsTo)
	{
		if (event.id == reactEvent)
			EventReaction(event);
	}
}
