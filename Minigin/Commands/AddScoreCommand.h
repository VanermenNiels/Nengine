#pragma once
#include <glm/glm.hpp>
#include "Command.h"

namespace dae
{
	class AddScoreCommand final : public Command
	{
	public:
		AddScoreCommand(GameObject* target, int scoreIncrement = 10);

		AddScoreCommand(const AddScoreCommand& other) = delete;
		AddScoreCommand(AddScoreCommand&& other) = delete;

		AddScoreCommand& operator=(const AddScoreCommand& other) = delete;
		AddScoreCommand& operator=(AddScoreCommand&& other) = delete;

		~AddScoreCommand() override = default;

		void Execute(float deltaTime) override;

	private:
		int m_ScoreIncrement;
	};
}
