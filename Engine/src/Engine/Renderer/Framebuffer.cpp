#include "Framebuffer.h"
#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace Polyboid
{
	Framebuffer::Framebuffer(const FramebufferSettings& settings) : m_Settings(settings)
	{
		ReCreateFramebuffer();
	
	}

	void Framebuffer::ReCreateFramebuffer()
	{

		if (m_ID)
		{

			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachment = 0;
			m_DepthAttachment = 0;
		}


		glCreateFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);


		Utils::CreateTexture(&m_ColorAttachment);
		Utils::BindTexture(m_ColorAttachment);
		Utils::AttachColorTexture(m_Settings.width, m_Settings.height, m_ColorAttachment);


		Utils::CreateTexture(&m_DepthAttachment);
		Utils::BindTexture(m_DepthAttachment);
		Utils::AttachDepthTexture(m_Settings.width, m_Settings.height, m_DepthAttachment);
	
	
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(1, buffers);


		if (glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			spdlog::error("Framebuffer is incomplete");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glViewport(0, 0, m_Settings.width, m_Settings.height);
	}


	void Framebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	std::shared_ptr<Framebuffer> Framebuffer::MakeFramebuffer(const FramebufferSettings& settings)
	{
		return std::make_shared<Framebuffer>(settings);
	}



}