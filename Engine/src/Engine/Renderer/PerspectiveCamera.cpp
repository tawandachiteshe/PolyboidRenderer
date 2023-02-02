#include "boidpch.h"
#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "glm/fwd.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Polyboid 
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearPlane, float farPlane)
	{

		m_Projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);

		const glm::mat4 rotation = glm::toMat4(glm::quat({glm::radians(45.0f), 0.0, 0.0f}));
		m_ViewMatrix = glm::mat4(1.0f);
		
	}
}
