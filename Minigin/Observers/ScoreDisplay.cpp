#include "ScoreDisplay.h"
#include "TextComponent.h"
#include "ScoreComponent.h"

dae::ScoreDisplay::ScoreDisplay(TextComponent* text, ScoreComponent* score)
	: m_TextCompRPtr{ text }, 
	  m_ScoreCompRPtr{ score }
{
}

void dae::ScoreDisplay::onNotify(GameObject*, Event event)
{
	if (event.id == make_sdbm_hash("ScoreChanged"))
		m_TextCompRPtr->SetText("Score: " + std::to_string(m_ScoreCompRPtr->GetScore()));
}
