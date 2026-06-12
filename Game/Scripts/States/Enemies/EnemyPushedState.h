#include "EnemyState.h"
#include "Commands/MoveCommand.h"
#include <memory>
#include "Components/GridComponent.h"
namespace dae
{
	class PengoGridComponent;
	class EnemyPushedState final : public EnemyState
	{
	public:
		EnemyPushedState(glm::vec2 startPos ,Direction dir, float speed, PengoGridComponent* grid, bool isPlayer = false) :
			EnemyState(), m_StartPos{ startPos }, m_Speed{speed}, m_GridRPtr{grid} { m_CurrentDir = dir; }

		~EnemyPushedState() override = default;
		void OnEnter(StateComponent& ctx, GameObject* gO) override;
		void Update(StateComponent& ctx, GameObject* gO, float deltaTime) override;

		 bool BeingPushed() const override { return true; }

	private:
		float m_Speed;
		std::unique_ptr<MoveCommand> m_MoveCommand{};
		PengoGridComponent* m_GridRPtr;
		glm::vec2 m_StartPos;
	};
}