#pragma once
#include "Framebuffer.h"
#include "Engine/Engine/Base.h"
#include "glm/vec4.hpp"


namespace Polyboid
{
    class Texture;
    

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
    	glm::vec4 color = glm::vec4(0);
        float depth = 1.0f;
        uint8_t stencil = 0;
        
    };

    struct RenderTargetTextureAttachment
    {
        TextureAttachmentSlot slot;
        TextureSizedInternalFormat format;
    };

    struct RenderTargetSettings
    {

        //Maybe add more textures here. IDK still working on it
        
        uint32_t Width = 0;
        uint32_t Height = 0;
        std::vector<RenderTargetTextureAttachment> TextureAttachments;
    };
    
    class RenderTarget
    {
    public:
        
        virtual void AttachTexture( TextureAttachmentSlot attachment, Ref<Texture> texture ) = 0;
        virtual Ref<Texture> GetTexture( TextureAttachmentSlot attachment ) = 0;

        virtual void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) = 0;
        virtual void Clear(const ClearSettings& settings) = 0;
        virtual void GenerateMipMaps() = 0;

        virtual ~RenderTarget() = default; 

    };

}
