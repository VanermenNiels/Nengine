#include "BaseComponent.h"
#include "../Variables.h"
#include "Commands/MoveCommand.h"
#include <memory>
#include "Components/GridComponent.h"

namespace dae
{
	class AnimatorComponent;
	class PengoGridComponent;
	class BlockComponent : public BaseComponent
	{
	public:
		BlockComponent(GameObject* pOwner,GridComponent::Cell currentCell, CellType type, PengoGridComponent* pGrid): 
			BaseComponent(pOwner), m_GridRPtr{ pGrid }, m_CurrentCell{ currentCell }, m_START_CELL{ currentCell }, m_Type {type} {}

		void Update(float deltaTime) override;
		void Push(Direction dir);
		// Used later for enemies when they overlap to check if a block is currently moving.
		Direction IsMoving() const { return m_CurrentDir; }
	private:
		PengoGridComponent* m_GridRPtr;
		Direction m_CurrentDir { Direction::Static };
		std::unique_ptr<MoveCommand> m_MoveCommand{};

		GridComponent::Cell m_CurrentCell;
		const GridComponent::Cell m_START_CELL;
		CellType m_Type;

		AnimatorComponent* m_AnimatorRPtr{};
		static const float SPEED;
	};
}