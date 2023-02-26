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

	struct FramebufferSettings
	{
		std::vector<TextureAttachmentSlot> attachmentSlots;
		uint32_t width = 0;
		uint32_t height = 0;
	};
	
	class Framebuffer
	{
	public:

		virtual void AttachRenderbuffer(const Ref<Renderbuffer>& renderbuffer, const TextureAttachmentSlot& slot) = 0;

		virtual void AttachTexture(const Ref<Texture>& texture, const TextureAttachmentSlot& slot) = 0;

		virtual void AttachTexture3D(const Ref<Texture3D>& texture, const TextureAttachmentSlot& slot) = 0;

		virtual ~Framebuffer() = default;
		
		virtual void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) = 0;
		virtual void Clear(const ClearSettings& settings) = 0;

		static  Ref<Framebuffer> Create(const FramebufferSettings& settings);
		
	};

}

