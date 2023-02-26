#include "boidpch.h"
#include "RenderAPI.h"

#include <intrin.h>

#include "Engine/Engine/Application.h"
#include "Platform/Opengl/GLRenderAPI.h"

namespace Polyboid
{

	Ref<RenderAPI> RenderAPI::Create(const RenderAPIType& renderType, const std::any& nativeWindow)
	{
		switch (renderType)
		{
		case RenderAPIType::Opengl: return std::make_shared<GLRenderAPI>(nativeWindow);
		case RenderAPIType::Vulkan: 
		case RenderAPIType::Metal:  
		case RenderAPIType::Dx11:
		case RenderAPIType::Dx12:
			__debugbreak();
			return nullptr;
		}

		__debugbreak();
		return nullptr;
	}
}
