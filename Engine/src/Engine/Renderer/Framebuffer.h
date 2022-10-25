#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <memory>

namespace Polyboid 
{
	enum class FramebufferTextureFormat
	{
		DEPTH,
		DEPTH24_STENCIL8,
		RGBA8,
		R32I,
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

		static void AttachColorTexture(uint32_t width, uint32_t height, uint32_t texture, uint32_t attachmentIndex = 0)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D, texture, 0);
		}

		static void AttachDepthTexture(uint32_t width, uint32_t height, uint32_t texture)
		{
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
				GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		}

		static void AttachTexture(uint32_t height, uint32_t width, FramebufferTextureFormat format, uint32_t* out, uint32_t colorIndex = 0)
		{
			uint32_t* texture = out;
			CreateTexture(texture);
			BindTexture(*texture);


			switch (format)
			{
			case FramebufferTextureFormat::DEPTH:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, GL_DEPTH_COMPONENT, 0, GL_UNSIGNED_BYTE, nullptr);
				break;
			case FramebufferTextureFormat::DEPTH24_STENCIL8:
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
				break;
			case FramebufferTextureFormat::RGBA8:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, GL_RGBA, 0, GL_UNSIGNED_BYTE, nullptr);
				break;
			case FramebufferTextureFormat::R32I:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, GL_RED, 0, GL_UNSIGNED_BYTE, nullptr);
				break;
			default:
				break;
			}

			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



			if (format == FramebufferTextureFormat::RGBA8)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, *texture, 0);
			}
			else if (format == FramebufferTextureFormat::DEPTH24_STENCIL8)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *texture, 0);
			}

			

			glBindTexture(GL_TEXTURE_2D, 0);

		}
	}

	struct FramebufferSettings {
		 // might need this FramebufferTextureFormat format;
		uint32_t width, height;

		FramebufferSettings(const FramebufferSettings& settings): width(settings.width), height(settings.height) {}
		FramebufferSettings(uint32_t width, uint32_t height): width(width), height(height) {}
	};

	class Framebuffer
	{

	private:
		uint32_t m_ID;

		//might need it maybe.... idk im not reading
		// might need multi sampled
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;

		FramebufferSettings m_Settings;

	
		//supports one texture format RGBA
		//look whats required for this to work
	public:
		Framebuffer(const FramebufferSettings& settings);

		//Recreate Buffer attachments
	public:
		void ReCreateFramebuffer();
		void Bind();
		void UnBind();
		static std::shared_ptr<Framebuffer> MakeFramebuffer(const FramebufferSettings& settings);
		uint32_t GetColorAttachment0TextureID() { return m_ColorAttachment; }

	};

}

