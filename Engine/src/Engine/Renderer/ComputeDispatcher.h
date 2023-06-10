#pragma once
#include "Engine/Engine/Base.h"
#include "glm/vec3.hpp"


namespace Polyboid
{
	class Shader;
	class Texture2D;
	class ShaderBufferStorage;

	class ComputeDispatcher
	{
	public:

		static void Begin();

		static void Dispatch(const glm::uvec3& threadGroup);

		static void End();

	};

	
}
