#include "boidpch.h"
#include "Renderbuffer.h"
#include "glad/glad.h"
#include <spdlog/spdlog.h>

#include "Texture2D.h"
#include "Texture3D.h"

namespace Polyboid
{
	Renderbuffer::Renderbuffer(uint32_t width, uint32_t height): m_Width(width), m_Height(height)
	{
	
		glCreateRenderbuffers(1, &m_ColorRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_ColorRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glCreateRenderbuffers(1, &m_DepthRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glCreateFramebuffers(1, &m_Framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, m_DepthRenderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorRenderbuffer);
		

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			spdlog::error("Error Incomplete Framebuffer");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	Renderbuffer::~Renderbuffer()
	{

		glDeleteRenderbuffers(1, &m_ColorRenderbuffer);
		glDeleteRenderbuffers(1, &m_DepthRenderbuffer);
		glDeleteFramebuffers(1, &m_Framebuffer);
	}

	void Renderbuffer::WriteTexture3D(uint32_t cubeFaceIndex, const Ref<Texture3D>& texture3D, uint32_t mipLevel)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeFaceIndex, texture3D->GetID(), mipLevel);
	}

	void Renderbuffer::WriteTexture2D(const Ref<Texture>& texture, const RenderbufferTextureSettings& textureSetting)
	{
		switch (textureSetting.format)
		{
		case RenderbufferTexture::Color:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textureSetting.colorAttachment,
				GL_TEXTURE_2D, texture->GetTextureID(), 0);
			break;
		case RenderbufferTexture::Depth:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
				GL_TEXTURE_2D, texture->GetTextureID(), 0);
			break;
		}
		
	}

	void Renderbuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
		glViewport(0, 0, m_Width, m_Height);
	}

	void Renderbuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderbuffer::ReSize(uint32_t width, uint32_t height)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);


		glViewport(0, 0, width, height);

	}

	Ref<Renderbuffer> Renderbuffer::MakeRenderBuffer(uint32_t width, uint32_t height )
	{
		return  std::make_shared<Renderbuffer>(width, height);
	}
}
