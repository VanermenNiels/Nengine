#pragma once

class GameObject;

namespace dae
{
	class Component
	{
	public:
		Component(GameObject* pOwner);

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;

		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual ~Component() {};

		// Will be empty for base class
		virtual void Update(float deltaTime);
		virtual void Render() const;

	protected:
		GameObject* m_pOwner;
	};
}
