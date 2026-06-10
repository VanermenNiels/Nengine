
namespace dae
{
	 class StateComponent;
	 class GameObject;
	 class State
	 {
	 public:
		 State() = default;
		 virtual ~State() = default;
		 virtual void OnEnter(StateComponent& ctx, GameObject* gO) = 0;
		 virtual void OnExit(StateComponent& ctx, GameObject* gO) = 0;
		 virtual void Update(StateComponent& ctx, GameObject* gO, float deltaTime) = 0;
	 };
}