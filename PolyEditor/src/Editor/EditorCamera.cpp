#include "EditorCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Polyboid 
{
	
	EditorCamera::EditorCamera(float fov, float aspectRatio, float near, float far)
	{

		m_View = glm::perspective(glm::radians(fov), aspectRatio, near, far);

	}


}