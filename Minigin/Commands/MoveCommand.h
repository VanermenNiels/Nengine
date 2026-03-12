#pragma once
#include <glm/glm.hpp>
#include "Command.h"

namespace dae
{
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* target, glm::vec3 dir, float speed);

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) = delete;

		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) = delete;

		~MoveCommand() override = default;

		void Execute(float deltaTime) override;

		void SetDirection(const glm::vec3& newDir) { m_Direction = newDir; }
		void SetSpeed(float newSpeed) { m_Speed = newSpeed; }
	private:
		float m_Speed{};
		glm::vec3 m_Direction{};
	};
}