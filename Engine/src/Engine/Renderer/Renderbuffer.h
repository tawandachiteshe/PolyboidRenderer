#pragma once
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class Texture;
	class Texture3D;

	enum class RenderbufferTexture
	{
		Color,
		Depth
	};

	struct RenderbufferTextureSettings
	{
		RenderbufferTexture format = RenderbufferTexture::Color;
		uint32_t colorAttachment = 0;

		RenderbufferTextureSettings(const RenderbufferTexture format, const uint32_t color_attachment)
			: format(format),
			  colorAttachment(color_attachment)
		{
		}
	};

	class Renderbuffer
	{

	private:
		uint32_t m_ColorRenderbuffer = 0;
		uint32_t m_DepthRenderbuffer = 0;
		uint32_t m_Framebuffer = 0;

		uint32_t m_Width = 0, m_Height = 0;


	public:

		Renderbuffer(uint32_t width, uint32_t height);
		~Renderbuffer();

		void WriteTexture3D(uint32_t cubeFaceIndex, const Ref<Texture3D>& texture3D, uint32_t mipMapLevel = 0);
		void WriteTexture2D(const Ref<Texture>& texture, const RenderbufferTextureSettings& textureSettings = { RenderbufferTexture::Color, 0 });

		void Bind();
		void UnBind();
		void ReSize(uint32_t width, uint32_t height);

		static Ref<Renderbuffer> MakeRenderBuffer(uint32_t width, uint32_t height);

	};
}
