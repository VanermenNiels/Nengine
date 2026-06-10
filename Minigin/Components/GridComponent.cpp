#include "GridComponent.h"
#include "Renderer.h"
#include "GameObject.h"

void dae::GridComponent::Render() const
{
	//mainly for debugging purposes, so we can see the grid and how the objects are placed on it
	if (!m_ShowGrid) return;

	const auto& renderer { Renderer::GetInstance() };
	for (int row{}; row <= m_Rows; ++row)
	{
		const float y { row * m_CellSize };
		renderer.RenderLine(0, y, m_Cols * m_CellSize, y, m_GridColor);
	}
	for (int col{}; col <= m_Cols; ++col)
	{
		const float x { col * m_CellSize };
		renderer.RenderLine(x, 0, x, m_Rows * m_CellSize, m_GridColor);
	}
}

dae::GridComponent::Cell dae::GridComponent::WorldToCell(glm::vec2 worldPos)
{
	const auto& origin = GetOwner()->GetWorldPosition();

	int col { static_cast<int>((worldPos.x - origin.x) / m_CellSize) };
	int row { static_cast<int>((worldPos.y - origin.y) / m_CellSize) };

	return Cell{ row, col };
}

glm::vec2 dae::GridComponent::CellToWorld(Cell cell)
{
	const auto& origin{ GetOwner()->GetWorldPosition() };

	return glm::vec2{
		origin.x + cell.col * m_CellSize,
		origin.y + cell.row * m_CellSize
	};
}

std::vector<glm::vec2> dae::GridComponent::GetBounds() const
{
	std::vector<glm::vec2> bounds{};

	const auto width{ m_Cols * m_CellSize };
	const auto height{ m_Rows * m_CellSize };

	const auto& topLeft{ GetOwner()->GetWorldPosition() };

	auto topRight{ topLeft};
	topRight.x += width;

	auto bottomLeft{ topLeft };
	bottomLeft.y += height;

	auto bottomRight{ bottomLeft };
	bottomRight.x += width;
	

	bounds.push_back(topLeft);
	bounds.push_back(topRight);
	bounds.push_back(bottomLeft);
	bounds.push_back(bottomRight);

	return bounds;
}

bool dae::GridComponent::IsInBounds(Cell cell) const
{
	return cell.row >= 0 && cell.row < m_Rows &&
		cell.col >= 0 && cell.col < m_Cols;
}
