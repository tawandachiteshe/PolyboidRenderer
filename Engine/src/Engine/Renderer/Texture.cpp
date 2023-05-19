#include "boidpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Engine/Engine/Application.h"

namespace Polyboid
{
	Ref<Texture> Texture::Create(const TextureSettings& settings)
	{
		return RenderAPI::Get()->CreateTexture2D(settings);
	}

	Ref<Texture> Texture::Create(const TextureSettings& settings, const void* data)
	{
		return RenderAPI::Get()->CreateTexture2D(settings, data);
	}
}
