#pragma once
#include "BaseComponent.h"
#include <SDL3/SDL_pixels.h>
#include <glm/glm.hpp>

namespace dae
{
	class GridComponent : public BaseComponent
	{
	public:

		struct Cell
		{
			int row{};
			int col{};
		};

		GridComponent(GameObject* pOwner, int rows, int cols, float cellSize) : 
			BaseComponent(pOwner), m_Rows {rows}, m_Cols {cols}, m_CellSize {cellSize} {}
		
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) noexcept = delete;

		virtual ~GridComponent() override = default;
		
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) noexcept = delete;

		void Render() const override;

		int GetRows() const { return m_Rows; }
		int GetCols() const { return m_Cols; }
		float GetCellSize() const { return m_CellSize; }

		void ShowGrid(SDL_Color color) { m_ShowGrid = true; m_GridColor = color; }
		void DisableRender() { m_ShowGrid = false;}

		Cell WorldToCell(glm::vec2 worldPos);
		glm::vec2 CellToWorld(Cell cell);

		std::vector<glm::vec2> GetBounds() const;

	protected:
		const int m_Rows;
		const int m_Cols;
		const float m_CellSize;

		bool IsInBounds(Cell cell) const;
	private:
		SDL_Color m_GridColor{};
		bool m_ShowGrid{};
	};
}