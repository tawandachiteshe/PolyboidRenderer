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


		static  glm::vec3 TransformVector(const glm::mat4& mat, const glm::vec3& vec)
		{
			glm::vec3 result{0};

			result.x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z;
			result.y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z;
			result.z = mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z;

			return  result;
		}

	}
}
