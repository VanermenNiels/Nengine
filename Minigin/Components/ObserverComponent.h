#pragma once
#include "BaseComponent.h"
#include "Observers/Observer.h"
#include "Events/Subject.h"

namespace dae
{
	class ObserverComponent : public BaseComponent, public Observer
	{
	public:
		ObserverComponent(GameObject* ownerRPtr, std::vector<EventId> reactsTo = {});

		virtual ~ObserverComponent() = default;

		void onNotify(GameObject*, Event event) override;

		void AddObserver(Observer* o) { m_Subject.AddObserver(o); }
		void RemoveObserver(Observer* o) { m_Subject.RemoveObserver(o); }

		void AddEventToReact(EventId event) { m_ReactsTo.push_back(event); }

	protected:
		// Use this in later derived classes to react to events.
		virtual void EventReaction(Event) {};
		Subject m_Subject{};
	private:
		std::vector<EventId> m_ReactsTo{};
	};
}