#pragma once
#include "BaseComponent.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class ScoreComponent final : public BaseComponent
	{
	public:
		ScoreComponent(GameObject* owner, TextComponent* text, int score = 0);

		void Update(float deltaTime) override;

		void AddScore(int scoreToAdd) { m_Score += scoreToAdd; }
		int GetScore() const { return m_Score; }
	private:
		TextComponent* m_TextComponentRPtr;
		int m_Score;
	};
}