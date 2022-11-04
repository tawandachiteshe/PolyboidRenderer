#pragma once
#include <glm/ext/matrix_float4x4.hpp>

namespace Polyboid
{
	// credit hazel engine the cherno
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	};
}