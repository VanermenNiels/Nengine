#pragma once
#include "EnemyState.h"
#include "Commands/MoveCommand.h"
#include "../Variables.h"
#include <memory>
#include <random>
#include "Components/GridComponent.h"

namespace dae
{
	class PengoGridComponent;
	class EnemyWalkingState : public EnemyState
	{
	public:
		EnemyWalkingState(Direction dir, PengoGridComponent* grid, bool isPlayer = false);

		~EnemyWalkingState() override = default;
		void OnEnter(StateComponent& ctx, GameObject* gO) override;
		void Update(StateComponent& ctx, GameObject* gO, float deltaTime) override;

	protected:
		int m_MaxBlocksToDestroy{ 2 };

	private:
		PengoGridComponent* m_GridRPtr;
		bool m_IsPlayer;
		bool m_HasLineOfSight{ false };
		int m_BlocksDestroyed{ 0 };
		std::unique_ptr<MoveCommand> m_MoveCommand{};
		static constexpr float SPEED{ 100.f };
		static constexpr float RANDOM_MOVE_CHANCE{ 0.4f };

		std::mt19937 m_Rng{ std::random_device{}() };

		static constexpr float TURN_CHANCE{ 0.6f };
		int CountExits(GridComponent::Cell cell);

		void AIMoving(StateComponent& ctx, GameObject* gO);
		bool HasLineOfSight(GridComponent::Cell from, GridComponent::Cell to);
		Direction GetChaseDirection(GridComponent::Cell enemyCell, GridComponent::Cell playerCell);
		Direction GetPathfindDirection(GridComponent::Cell enemyCell, GridComponent::Cell playerCell);
		Direction GetRandomDirection(GridComponent::Cell enemyCell);
	};
}