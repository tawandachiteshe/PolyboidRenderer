#pragma once
#include "Engine/Engine/Base.h"
#include "glm/vec3.hpp"


namespace Polyboid
{
	class Shader;
	class Texture;
	class ShaderBufferStorage;

	class ComputeRenderer
	{
	public:

		static void Begin();

		static void WriteToTexture(const Ref<Texture>& texture, const Ref<Shader>& computeShader, uint32_t textureSlot = 0);

		static void WriteToBuffer(const Ref<ShaderBufferStorage>& storage, const Ref<Shader>& computeShader,
			glm::uvec3 threadGroup);

		static void End();

	};

	
}
