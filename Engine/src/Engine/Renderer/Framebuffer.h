#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <memory>

#include "glm/vec4.hpp"

namespace Polyboid 
{
	enum class FramebufferTextureFormat
	{
		DEPTH,
		DEPTH24_STENCIL8,
		RGBA8,
		RGBA32F,
		R32I,
		R32F
	};


	namespace Utils 
	{

		static void CreateTexture(uint32_t* out)
		{
		
			glCreateTextures(GL_TEXTURE_2D, 1, out);
		}

		static void BindTexture(uint32_t texture)
		{

			glBindTexture(GL_TEXTURE_2D, texture);
		}

		static void AttachColorTexture(uint32_t width, uint32_t height, const FramebufferTextureFormat& format,uint32_t texture, uint32_t attachmentIndex = 0)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, nullptr);
				break;
			case FramebufferTextureFormat::DEPTH24_STENCIL8:
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
				break;
			case FramebufferTextureFormat::RGBA8:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				break;
			case FramebufferTextureFormat::R32I:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED, GL_INT, nullptr);
				break;
			case FramebufferTextureFormat::R32F:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, nullptr);
				break;
			case FramebufferTextureFormat::RGBA32F:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
				break;
			}

			glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D, texture, 0);
		}

		static void AttachDepthTexture(uint32_t width, uint32_t height, uint32_t texture)
		{
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
				GL_DEPTH_COMPONENT, GL_FLOAT, NULL
			);

			glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		}

	}


	struct FramebufferTexture
	{
		FramebufferTextureFormat format;
	};

	struct FramebufferSettings {
		 // might need this FramebufferTextureFormat format;
		uint32_t width, height;

		std::vector<FramebufferTexture> m_Textures;
		FramebufferSettings() = default;

		FramebufferSettings(const FramebufferSettings& settings): width(settings.width), height(settings.height), m_Textures(settings.m_Textures) {}
		FramebufferSettings(uint32_t width, uint32_t height, const std::initializer_list<FramebufferTexture>& textures = {}): width(width), height(height), m_Textures(textures) {}
	};

	class Framebuffer
	{

	private:
		uint32_t m_ID;

		//might need it maybe.... idk im not reading
		// might need multi sampled
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;

		std::vector<uint32_t> m_ColorAttachments;
		FramebufferSettings m_Settings;

	
		//supports one texture format RGBA
		//look whats required for this to work
	public:
		Framebuffer(const FramebufferSettings& settings);

		//Recreate Buffer attachments
	public:
		void ReCreateFramebuffer();
		void Resize(uint32_t width, uint32_t height);
		void Bind();
		void ClearColor(uint32_t attachmentIndex = 0 , const glm::vec4& color = { 0.2, 0.2, 0.2, 1.0f });
		void BindColorAttachments(uint32_t index = 0, uint32_t textureIndex = 0);
		void BindDepthAttachment(uint32_t textureIndex = 0);
		void UnBind();
		static std::shared_ptr<Framebuffer> MakeFramebuffer(const FramebufferSettings& settings);
		std::vector<uint32_t>& GetColorAttachments() { return m_ColorAttachments; }

		FramebufferSettings& GetSettings() { return m_Settings; }

		uint32_t GetColorAttachment0()
		{
			if (m_ColorAttachments.empty())
			{
				return 0;
			}

			return m_ColorAttachments[0];
		}
		uint32_t GetDepthAttachment() { return m_DepthAttachment; }

	};

}

