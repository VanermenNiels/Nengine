#pragma once
#include "Command.h"
#include "Events/Event.h"
#include "Events/Subject.h"

namespace dae
{
	class EventCommand final : public Command
	{
	public:
		EventCommand(GameObject* target, Subject* subject, unsigned int eventId);

		void Execute(float) override;
	private:
		Subject* m_pSubject{};
		unsigned int m_EventId{};
	};
}