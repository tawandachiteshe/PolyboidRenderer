#pragma once
#include <any>

#include "Texture.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class Semaphore;
	class RenderPass;
	//We support only srbg

	struct SwapchainSettings
	{
        SwapchainSettings() = default;

        uint32_t Width = 1;
        uint32_t Height = 1;

        EngineGraphicsFormats SwapchainFormat = EngineGraphicsFormats::BGRA8ISrgb;

	};

    class Swapchain
    {
    public:
        virtual uint32_t GetImageIndex(const Ref<Semaphore>& imageSemaphore) = 0;
        virtual void Present(const Ref<Semaphore>& renderSemaphore) = 0;
        virtual void SetVsync(bool vsync)= 0;
        virtual ~Swapchain() = default;
        virtual void Resize(uint32_t width = 0, uint32_t height = 0) = 0;
        virtual Ref<RenderPass> GetDefaultRenderPass() = 0;

        //TODO: Make it robust
        static Ref<Swapchain> Create(const SwapchainSettings& settings);
    };

}
