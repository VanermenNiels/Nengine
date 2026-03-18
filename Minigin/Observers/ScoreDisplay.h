#pragma once
#include "Observer.h"
#include "Events/Event.h"

namespace dae
{
	class TextComponent;
	class ScoreComponent;
	class ScoreDisplay final : public Observer
	{
	public:
		ScoreDisplay(TextComponent* text, ScoreComponent* score);

		void onNotify(GameObject*, Event event) override;

	private:
		TextComponent* m_TextCompRPtr;
		ScoreComponent* m_ScoreCompRPtr;
	};
}