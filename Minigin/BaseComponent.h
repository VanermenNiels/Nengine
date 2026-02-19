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

		bool IsEnabled() const { return m_Enabled; }
		void SetEnabled(bool enabled) { m_Enabled = enabled; }

	protected:
		GameObject* GetOwner() const { return m_OwnerRPtr; }
	private:
		GameObject* m_OwnerRPtr;
		bool m_Enabled { true };
	};
}
