#include "EventCommand.h"
#include "ScoreComponent.h"
#include "GameObject.h"


dae::EventCommand::EventCommand(GameObject* target, Subject* subject, unsigned int eventId):
	Command(target), 
	m_pSubject(subject), 
	m_EventId(eventId)
{
}

void dae::EventCommand::Execute(float)
{
	if (m_pSubject)
		m_pSubject->Notify(m_TargetGameObjectRPtr, Event{ m_EventId });
}
