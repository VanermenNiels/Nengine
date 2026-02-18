#pragma once
namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* pOwner);

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;

		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual ~BaseComponent() {};

		// Will be empty for base class
		virtual void Update(float deltaTime);
		virtual void Render() const;

	protected:
		GameObject* GetOwner() const { return m_OwnerRPtr; }
	private:
		GameObject* m_OwnerRPtr;
	};
}
