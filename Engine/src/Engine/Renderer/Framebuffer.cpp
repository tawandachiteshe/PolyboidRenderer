#include "boidpch.h"

#include "Framebuffer.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "Engine/Engine/Debug/Profiler.h"
#include "glm/gtc/type_ptr.hpp"

namespace Polyboid
{
	Framebuffer::Framebuffer(const FramebufferSettings& settings) : m_Settings(settings)
	{
		POLYBOID_PROFILE_FUNCTION();
		ReCreateFramebuffer();
	
	}

	void Framebuffer::ReCreateFramebuffer()
	{
		POLYBOID_PROFILE_FUNCTION();

		
		if (m_ID)
		{
		
			glDeleteFramebuffers(1, &m_ID);
		
			if (!m_ColorAttachments.empty())
			{
				for (auto texture : m_ColorAttachments)
				{
					glDeleteTextures(1, &texture);
				}
			}
		
			
			glDeleteTextures(1, &m_DepthAttachment);
			glDeleteSamplers(1, &m_DepthSampler);
		
			m_ColorAttachment = 0;
			m_DepthAttachment = 0;
			m_DepthSampler = 0;
			m_ColorAttachments.clear();
		}


		for (auto element : m_Settings.m_Textures)
		{
			GLuint texture_map;
			glGenTextures(1, &texture_map);
			glBindTexture(GL_TEXTURE_2D, texture_map);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


			switch (element.format)
			{
			case FramebufferTextureFormat::DEPTH:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Settings.width, m_Settings.height, 0, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, nullptr);
				break;
			case FramebufferTextureFormat::DEPTH24_STENCIL8:
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Settings.width, m_Settings.height);
				break;
			case FramebufferTextureFormat::RGBA8:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Settings.width, m_Settings.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				break;
			case FramebufferTextureFormat::R32I:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Settings.width, m_Settings.height, 0, GL_RED, GL_INT, nullptr);
				break;
			case FramebufferTextureFormat::R32F:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_Settings.width, m_Settings.height, 0, GL_RED, GL_FLOAT, nullptr);
				break;
			}

			glBindTexture(GL_TEXTURE_2D, 0);

			m_ColorAttachments.push_back(texture_map);

		}


		

		// Build the texture that will serve as the depth attachment for the framebuffer.
		glGenTextures(1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Settings.width, m_Settings.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);


		GLuint framebuffer;
		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)m_ID);

		uint32_t count = 0;
		for (auto element : m_ColorAttachments)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + count, GL_TEXTURE_2D, element, 0);

			count++;
		}

		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);


	
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

		auto drawCount = (GLsizei)m_ColorAttachments.size();

		if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}
		else
		{
			glDrawBuffers(drawCount, buffers);
		}

		

//		
		



		if (glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			spdlog::error("Framebuffer is incomplete framebuffer class");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		POLYBOID_PROFILE_FUNCTION();
		m_Settings.width = width;
		m_Settings.height = height;

		ReCreateFramebuffer();
	}

	void Framebuffer::Bind()
	{
		POLYBOID_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glViewport(0, 0, m_Settings.width, m_Settings.height);

	}

	void Framebuffer::ClearColor(uint32_t attachmentIndex , const glm::vec4& color)
	{
		glClearTexImage(m_ColorAttachments.at(attachmentIndex), 0, GL_RGBA, GL_FLOAT, glm::value_ptr(color));
	}

	void Framebuffer::BindColorAttachments(uint32_t index, uint32_t textureIndex)
	{
		if (!m_ColorAttachments.empty())
		{
			uint32_t texture = m_ColorAttachments.at(index);
			glBindTextureUnit(textureIndex, texture);
			glBindSampler(textureIndex, m_DepthSampler);
		}

	}

	void Framebuffer::BindDepthAttachment(uint32_t textureIndex)
	{
		glBindTextureUnit(textureIndex, m_DepthAttachment);
		glBindSampler(textureIndex, m_DepthSampler);
	}


	void Framebuffer::UnBind()
	{
		POLYBOID_PROFILE_FUNCTION();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	std::shared_ptr<Framebuffer> Framebuffer::MakeFramebuffer(const FramebufferSettings& settings)
	{
		POLYBOID_PROFILE_FUNCTION();
		return std::make_shared<Framebuffer>(settings);
	}



}
