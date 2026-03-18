#include "ScoreComponent.h"
#include "TextComponent.h"
#include <sstream>

dae::ScoreComponent::ScoreComponent(GameObject* owner, TextComponent* text, int score):
	BaseComponent(owner),
	m_TextComponentRPtr{ text },
	m_Score { score }
{
}

void dae::ScoreComponent::Update(float)
{
	std::stringstream ss;
	ss << std::fixed << "Score: " << m_Score;
	m_TextComponentRPtr->SetText(ss.str());
}
