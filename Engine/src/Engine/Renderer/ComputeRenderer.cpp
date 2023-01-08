#include "boidpch.h"
#include "ComputeRenderer.h"

#include "Shader.h"
#include "Texture2D.h"
#include "glad/glad.h"

namespace Polyboid
{
	void ComputeRenderer::Begin()
	{
	}

	void ComputeRenderer::WriteToTexture(const Ref<Texture>& texture, const Ref<Shader>& computeShader, uint32_t textureSlot)
	{
		texture->Bind(textureSlot, true);
		computeShader->Bind();

		glDispatchCompute(texture->GetWidth(), texture->GetHeight(), 1);
	}

	void ComputeRenderer::End()
	{
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
}
