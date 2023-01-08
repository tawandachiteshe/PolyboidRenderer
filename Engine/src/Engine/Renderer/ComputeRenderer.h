#pragma once
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class Shader;
	class Texture;

	class ComputeRenderer
	{
	public:

		static void Begin();

		static void WriteToTexture(const Ref<Texture>& texture, const Ref<Shader>& computeShader, uint32_t textureSlot = 0);

		static void End();

	};

	
}
