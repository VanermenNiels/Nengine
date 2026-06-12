#include "EnemyState.h"
namespace dae
{
	class EnemySquishedState : public EnemyState
	{
	public:
		EnemySquishedState(Direction dir) { m_CurrentDir = dir; }
		~EnemySquishedState() override = default;
		void OnEnter(StateComponent& ctx, GameObject* gO) override;
		void Update(StateComponent& ctx, GameObject* gO, float deltaTime) override;
	private:
	};
}