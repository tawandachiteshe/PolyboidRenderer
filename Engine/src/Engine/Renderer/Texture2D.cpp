#include "boidpch.h"
#include "Texture2D.h"

#include "RenderCommand.h"
#include "Engine/Engine/Application.h"

namespace Polyboid
{
	Ref<Texture2D> Texture2D::Create(const TextureSettings& settings)
	{
		return RenderAPI::Get()->CreateTexture2D(settings);
	}

	Ref<Texture2D> Texture2D::Create(const TextureSettings& settings, const void* data)
	{
		return RenderAPI::Get()->CreateTexture2D(settings, data);
	}

	Ref<Texture2D> Texture2D::Create(const Ref<Image2D>& image)
	{
		return RenderAPI::Get()->CreateTexture2D(image);
	}

	Ref<Texture3D> Texture3D::Create(const TextureSettings& settings, const void* data)
	{
		return RenderAPI::Get()->CreateTexture3D(data, settings);
	}
}
