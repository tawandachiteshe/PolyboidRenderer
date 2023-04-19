#include "boidpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Engine/Engine/Application.h"

namespace Polyboid
{
	Ref<Texture> Texture::Create(const TextureSettings& settings)
	{
		const auto renderApi = Application::Get().GetRenderAPI();

		return renderApi->CreateTexture2D(settings);

	}

	Ref<Texture> Texture::Create(const void* data, const TextureSettings& settings)
	{
		auto renderApi = Application::Get().GetRenderAPI()->GetRenderAPIType();

		switch (renderApi)
		{
		case RenderAPIType::Opengl:
		case RenderAPIType::Vulkan:
		case RenderAPIType::Metal:
		case RenderAPIType::Dx11:
		case RenderAPIType::Dx12:
			return nullptr;
		}

		return nullptr;
	}
}
