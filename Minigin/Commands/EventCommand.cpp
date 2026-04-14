#include "EventCommand.h"
#include "GameObject.h"

dae::EventCommand::EventCommand(GameObject* target, Observer* observer, unsigned int eventId, int value) :
	Command(target),
	m_ObserverRPtr{ observer },
	m_EventId{ eventId },
	m_Value{ value }
{}

void dae::EventCommand::Execute(float)
{
	if (!m_ObserverRPtr) return;

	Event event{ m_EventId };
	event.args[0].intVal = m_Value;
	event.nbArgs = 1;

	m_ObserverRPtr->onNotify(m_TargetGameObjectRPtr, event);
}