#pragma once
#include "Observer.h"
#include "Events/Event.h"

namespace dae
{
	class TextComponent;
	class HealthComponent;
	class HealthDisplay final : public Observer
	{
	public:
		HealthDisplay(TextComponent* text, HealthComponent* health);

		void onNotify(GameObject*, Event event) override;

	private:
		TextComponent* m_TextCompRPtr;
		HealthComponent* m_HealthCompRPtr;
	};
}