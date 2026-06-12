#include "BaseComponent.h"
#include "../Variables.h"
#include "Commands/MoveCommand.h"
#include <memory>
#include "Components/GridComponent.h"
#include <Events/Subject.h>

namespace dae
{
	class HitboxComponent;
	class AnimatorComponent;
	class PengoGridComponent;
	class EnemyStateComponent;
	class BlockComponent : public BaseComponent
	{
	public:
		BlockComponent(GameObject* pOwner,GridComponent::Cell currentCell, CellType type, PengoGridComponent* pGrid): 
			BaseComponent(pOwner), m_GridRPtr{ pGrid }, m_CurrentCell{ currentCell }, m_START_CELL{ currentCell }, m_Type {type} {}

		void Update(float deltaTime) override;
		void Push(Direction dir);
		void Destroy();
		// Used later for enemies when they overlap to check if a block is currently moving.
		Direction IsMoving() const { return m_CurrentDir; }

		Subject& GetSubject() { return m_Subject; }

	private:
		PengoGridComponent* m_GridRPtr;
		Direction m_CurrentDir { Direction::Static };
		std::unique_ptr<MoveCommand> m_MoveCommand{};

		GridComponent::Cell m_CurrentCell;
		const GridComponent::Cell m_START_CELL;
		CellType m_Type;

		Subject m_Subject{};

		AnimatorComponent* m_AnimatorRPtr{};

		HitboxComponent* m_Hitbox{};
		std::vector<HitboxComponent*> m_EnemyHitboxes{};
		std::vector<EnemyStateComponent*> m_EnemyStateComponents{};
		EnemyStateComponent* m_CurrentGrabbedEnemy{};
		static const float SPEED;
	};
}