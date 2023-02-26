#include "boidpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Engine/Engine/Application.h"
#include "Platform/Opengl/GLTexture2D.h"

namespace Polyboid
{
	Ref<Texture> Texture::Create(const TextureSettings& settings)
	{
		auto renderApi = Application::Get().GetRenderAPI()->GetRenderAPIType();

		switch (renderApi)
		{
			case RenderAPIType::Opengl: return std::make_shared<GLTexture2D>(settings);
			case RenderAPIType::Vulkan:
			case RenderAPIType::Metal:
			case RenderAPIType::Dx11:
			case RenderAPIType::Dx12:
			return nullptr;
		}

		return nullptr;

	}

	Ref<Texture> Texture::Create(const void* data, const TextureSettings& settings)
	{
		auto renderApi = Application::Get().GetRenderAPI()->GetRenderAPIType();

		switch (renderApi)
		{
		case RenderAPIType::Opengl: return std::make_shared<GLTexture2D>(data, settings);
		case RenderAPIType::Vulkan:
		case RenderAPIType::Metal:
		case RenderAPIType::Dx11:
		case RenderAPIType::Dx12:
			return nullptr;
		}

		return nullptr;
	}
}
