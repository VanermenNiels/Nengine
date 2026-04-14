#pragma once
#include <cstdint>
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

		uint32_t GetOwnerTag() const { return m_OwnerTag; }

	protected:
		GameObject* GetOwner() const { return m_OwnerRPtr; }
	private:
		GameObject* m_OwnerRPtr;
		const uint32_t m_OwnerTag;

		bool m_Enabled { true };
	};
}
