#include "FPSComponent.h"
#include "TextComponent.h"
#include <sstream>
#include <iomanip>

dae::FPSComponent::FPSComponent(GameObject* powner, TextComponent* textComponent):
	BaseComponent(powner), m_TextComponentRPtr{textComponent}
{
}

void dae::FPSComponent::Update(float deltaTime)
{
	m_FPS = 1.f / deltaTime;

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << m_FPS << " FPS";
	m_TextComponentRPtr->SetText(ss.str());
}
