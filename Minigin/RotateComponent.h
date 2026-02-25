#pragma once
#include "BaseComponent.h"
#include "Transform.h"
#include <numbers>

namespace dae
{
	class RotateComponent final : public BaseComponent
	{
	public:
		RotateComponent(GameObject* owner, float speed, float radius);

		RotateComponent(const RotateComponent& other) = delete;
		RotateComponent(RotateComponent&& other) = delete;

		RotateComponent& operator=(const RotateComponent& other) = delete;
		RotateComponent& operator=(RotateComponent&& other) = delete;

		virtual ~RotateComponent() = default;
		virtual void Update(float deltaTime) override;

		void SetRotationPoint(glm::vec3 rotationPoint) { m_RotationPoint = rotationPoint; }
	private:
		float m_Speed;
		float m_Radius;
		float m_Angle{};

		float m_PI{ static_cast<float>(std::numbers::pi) };

		glm::vec3 m_RotationPoint{};
	};
}