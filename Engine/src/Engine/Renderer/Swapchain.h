#pragma once
#include <any>

#include "Texture.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class Framebuffer;
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
        virtual void SetVsync(bool vsync)= 0;
        virtual ~Swapchain() = default;
        virtual void Resize(uint32_t width = 0, uint32_t height = 0) = 0;
        virtual Ref<RenderPass> GetRenderPass() = 0;
        virtual Ref<Framebuffer> GetCurrentFrameBuffer() = 0;
        virtual Ref<Framebuffer> GetFrameBuffer(uint32_t index) = 0;
        virtual std::vector<Ref<Framebuffer>> GetFrameBuffers() = 0;
        virtual std::any GetHandle() = 0;

        //TODO: Make it robust
        static Ref<Swapchain> Create(const SwapchainSettings& settings);
    };

}
