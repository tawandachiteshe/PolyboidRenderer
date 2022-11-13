#include "boidpch.h"
#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Polyboid 
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
	{

		m_Projection = glm::perspective(glm::radians(fov), aspect, near, far);

	}
}