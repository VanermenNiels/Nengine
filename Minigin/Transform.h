#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetWorldPosition() const { return m_WorldPos; }
		const glm::vec3& GetLocalPosition() const { return m_LocalPos; }

		void SetWorldPosition(float x, float y, float z = 0);
		void SetLocalPosition(float x, float y, float z = 0);

		void SetWorldPosition(const glm::vec3& position);
		void SetLocalPosition(const glm::vec3& position);

		void UpdateWorldPosition(float deltaX, float deltaY, float deltaZ = 0);
		void UpdateLocalPosition(float deltaX, float deltaY, float deltaZ = 0);
	private:
		glm::vec3 m_WorldPos;
		glm::vec3 m_LocalPos;
	};
}
