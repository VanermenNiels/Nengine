#pragma once
#include "Command.h"
#include "Events/Event.h"
#include "Observers/Observer.h"

namespace dae
{
	class EventCommand final : public Command
	{
	public:
		EventCommand(GameObject* target, Observer* observer, unsigned int eventId, int value = 0);

		void Execute(float) override;
	private:
		Observer* m_ObserverRPtr{};
		unsigned int m_EventId{};
		int          m_Value{};
	};
}