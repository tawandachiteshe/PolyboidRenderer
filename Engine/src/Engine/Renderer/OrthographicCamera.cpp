#include "boidpch.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Polyboid
{

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		m_Projection = glm::ortho(left, right, bottom, top);
	}
}
