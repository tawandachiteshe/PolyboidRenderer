#pragma once
#include "Framebuffer.h"
#include "Engine/Engine/Base.h"
#include "glm/vec4.hpp"


namespace Polyboid
{
	class Swapchain;
	class Texture2D;
    

    enum class ClearFlags : uint8_t
    {
        Color = 1 << 0,
        Depth = 1 << 1,
        Stencil = 1 << 2,
        DepthStencil = Depth | Stencil,
        All = Color | Depth | Stencil,
    };

    struct ClearSettings
    {

        ClearSettings() = default;
        ClearSettings(const glm::vec4& col): color(col){}

        ClearFlags clearFlags = ClearFlags::All;
    	glm::vec4 color = glm::vec4(0.2f);
        float depth = 1.0f;
        uint8_t stencil = 0;
        
    };

    enum class RenderPassType
    {
	    Present,
        ColorAttachment,
        Copy,
        ShaderReadOnly
    };

    struct RenderPassSettings
    {

        //Maybe add more textures here. IDK still working on it
        std::string debugName = "default";
        uint32_t Width = 0;
        uint32_t Height = 0;
        RenderPassType type = RenderPassType::Present;
        std::vector<TextureAttachment> TextureAttachments;
        bool IsSwapchainRenderPass = true;
    };
    
    class RenderPass : public RenderResource
    {
    public:

        virtual void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) = 0;
        virtual void Clear(const ClearSettings& settings) = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual Ref<Texture2D> GetColorTexture(const TextureAttachmentSlot& attachmentIndex, uint32_t frameIndex = 0) = 0;
        virtual Ref<Texture2D> GetDepthTexture(uint32_t frameIndex = 0) = 0;

        virtual ~RenderPass() = default;

        virtual RenderPassSettings& GetRenderPassSettings() = 0;

        static Ref<RenderPass> Create(const RenderPassSettings& settings);

    };

}
