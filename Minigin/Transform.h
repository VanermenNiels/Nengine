#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetWorldPosition() const { return m_WorldPosition; }
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }

		void SetWorldPosition(float x, float y, float z = 0);
		void SetLocalPosition(float x, float y, float z = 0);

		void UpdatePosition(float deltaX, float deltaY, float deltaZ = 0);

		void SetWorldPosition(const glm::vec3& position);
		void SetLocalPosition(const glm::vec3& localPosition);
	private:
		glm::vec3 m_WorldPosition{};
		glm::vec3 m_LocalPosition{};
	};
}
