#include "MoveCommand.h"
#include "GameObject.h"

dae::MoveCommand::MoveCommand(GameObject* target, glm::vec3 dir, float speed):
	Command(target),
	m_Speed { speed },
	m_Direction { dir }
{

}

void dae::MoveCommand::Execute()
{
	m_TargetGameObjectRPtr->UpdatePosition(m_Direction.x * m_Speed, m_Direction.y * m_Speed);
}
