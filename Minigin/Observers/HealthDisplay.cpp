#include "HealthDisplay.h"
#include "TextComponent.h"
#include "HealthComponent.h"

dae::HealthDisplay::HealthDisplay(TextComponent* text, HealthComponent* health):
	m_TextCompRPtr{ text },
	m_HealthCompRPtr{ health }
{
}

void dae::HealthDisplay::onNotify(GameObject*, Event event)
{
	if (event.id == dae::make_sdbm_hash("HealthChanged"))
	{
		m_TextCompRPtr->SetText("Lives: " + std::to_string(m_HealthCompRPtr->GetHealth()));
	}
}
