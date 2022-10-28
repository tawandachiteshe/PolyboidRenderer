#pragma once

#include <glm/glm.hpp>

namespace Polyboid 
{

	class EditorCamera
	{
	public:
		EditorCamera(float fov, float aspectRatio, float near, float far);
		~EditorCamera() = default;

	private:

		glm::mat4 m_Projection;
		glm::mat4 m_View;

	};

}