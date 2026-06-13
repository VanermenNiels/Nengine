#pragma once
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
        BlockComponent(GameObject* pOwner, GridComponent::Cell currentCell, CellType type, PengoGridComponent* pGrid) :
            BaseComponent(pOwner), m_GridRPtr{ pGrid }, m_CurrentCell{ currentCell }, m_START_CELL{ currentCell }, m_Type{ type } {}

        void Update(float deltaTime) override;
        void Push(Direction dir);
        void Destroy(bool killEnemy = true);

        void SetEnemyGO(GameObject* gO) { m_EnemyGO = gO; }
        Direction IsMoving() const { return m_CurrentDir; }
        bool IsDestroyed() const { return m_IsDestroyed; }
        bool IsHatchDestroy() const { return m_IsHatchDestroy; }
        void SetHatchDestroy() { m_IsHatchDestroy = true; }

        Subject& GetSubject() { return m_Subject; }

    private:
        PengoGridComponent* m_GridRPtr;
        Direction m_CurrentDir{ Direction::Static };
        std::unique_ptr<MoveCommand> m_MoveCommand{};
        std::unique_ptr<MoveCommand> m_EnemyMoveCommand{};

        GridComponent::Cell m_CurrentCell;
        const GridComponent::Cell m_START_CELL;
        CellType m_Type;

        Subject m_Subject{};

        AnimatorComponent* m_AnimatorRPtr{};

        HitboxComponent* m_Hitbox{};
        std::vector<HitboxComponent*> m_EnemyHitboxes{};
        std::vector<EnemyStateComponent*> m_EnemyStateComponents{};
        std::vector<EnemyStateComponent*> m_CurrentGrabbedEnemies;

        GameObject* m_EnemyGO{};

        bool m_IsDestroyed{ false };
        bool m_IsHatchDestroy{ false };

        static const float SPEED;
    };
}