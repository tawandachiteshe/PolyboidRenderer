#include "boidpch.h"
#include "RenderAPI.h"

#include <intrin.h>

#include "Engine/Engine/Application.h"
#include "Platform/Opengl/GLRenderAPI.h"
#include "Platform/Vulkan/VkRenderAPI.h"

namespace Polyboid
{

	RenderAPI* RenderAPI::s_RenderAPI = nullptr;

	RenderAPI* RenderAPI::Create(const RenderAPIType& renderType, const std::any& nativeWindow)
	{
		static RenderAPI* renderApi = nullptr;
		switch (renderType)
		{
		case RenderAPIType::Opengl: renderApi = new GLRenderAPI(nativeWindow); s_RenderAPI = renderApi; return renderApi;
		case RenderAPIType::Vulkan: renderApi = new VkRenderAPI(nativeWindow); s_RenderAPI = renderApi; return renderApi;
		case RenderAPIType::Metal:  
		case RenderAPIType::Dx11:
		case RenderAPIType::Dx12:
			__debugbreak();
			return nullptr;
		}

		__debugbreak();
		return nullptr;
	}

	RenderAPI* RenderAPI::Get()
	{
		if (s_RenderAPI)
		{

			__debugbreak();
		}

		return s_RenderAPI;
	}
}
