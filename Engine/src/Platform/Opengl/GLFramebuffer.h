#pragma once
#include "Engine/Renderer/Framebuffer.h"

namespace Polyboid
{
	class GLTexture2D;
	class GLRenderbuffer;

	class GLFramebuffer : public Framebuffer
	{
	private:
		FramebufferSettings m_Settings;
		uint32_t m_Handle = 0;
		std::map<TextureAttachmentSlot, Ref<GLTexture2D>> m_Textures;
		std::map<TextureAttachmentSlot, Ref<GLRenderbuffer>> m_Renderbuffers;

		bool m_Dirty = true;

		void ClearAllColors(const ClearSettings& settings);
		Ref<GLFramebuffer> m_Framebuffer;

	public:
		GLFramebuffer(const FramebufferSettings& settings);

		void AttachRenderbuffer(const Ref<Renderbuffer>& renderbuffer, const TextureAttachmentSlot& slot) override;
		void AttachTexture(const Ref<Texture>& texture, const TextureAttachmentSlot& slot) override;
		void AttachTexture3D(const Ref<Texture3D>& texture, const TextureAttachmentSlot& slot) override;

		virtual void Bind();
		virtual void UnBind();

		void ReSize(uint32_t width, uint32_t height) override;

		virtual Ref<Texture> GetTexture(TextureAttachmentSlot slot);

		void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) override;
		void Clear(const ClearSettings& settings) override;

		~GLFramebuffer() override;

		friend class GLRenderTarget;
	};
}
