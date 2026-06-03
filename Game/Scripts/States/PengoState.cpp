#include "PengoState.h"

dae::PengoState::PengoState(int player) : m_Player{ player } 
{
	m_StartRow = m_Player * TOTAL_ROWS_PER_PLAYER;
}
