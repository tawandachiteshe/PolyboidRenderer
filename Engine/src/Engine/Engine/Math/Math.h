#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace Polyboid
{
	namespace Math
	{

		static void DecomposeMatrix(glm::vec3& position, glm::vec3& scale, glm::quat& orientation, glm::mat4& mat)
		{
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(mat, scale, orientation, position, skew, perspective);
		}
		
	}
}
