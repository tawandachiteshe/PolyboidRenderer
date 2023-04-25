#pragma once
#include <cstdint>
#include <memory>

#include "Framebuffer.h"
#include "Renderbuffer.h"
#include "Engine/Engine/Base.h"

namespace Polyboid 
{
	struct ClearSettings;
	class Texture;
	
	
	enum class TextureAttachmentSlot
	{
		// Color attachments
		Color0 = 0,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		Color6,
		Color7,

		// Depth attachment
		Depth,

		// Stencil attachment
		Stencil,

		// Depth and stencil attachment
		DepthStencil
	};

	struct TextureAttachment
	{
		TextureAttachmentSlot slot;
		EngineGraphicsFormats format;
	};

	struct FramebufferSettings
	{
		std::vector<TextureAttachment> attachmentSlots;
		bool IsSwapChainUsage = true;
		uint32_t width = 0;
		uint32_t height = 0;
	};
	
	class Framebuffer
	{
	public:

		virtual ~Framebuffer() = default;
		
		virtual void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) = 0;
		virtual void Clear(const ClearSettings& settings) = 0;

		virtual void ReSize(uint32_t width, uint32_t height) =0;

		static  Ref<Framebuffer> Create(const FramebufferSettings& settings);
		
	};

}

