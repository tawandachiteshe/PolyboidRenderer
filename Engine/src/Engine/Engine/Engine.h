#pragma once
#include "Gameplay/World.h"


namespace Polyboid
{

	//Change Setttings n shit here

	struct EngineSettings
	{
		std::vector<Ref<World>> m_Worlds;
		Ref<Framebuffer> m_MainFrameBuffer;
		Ref<Framebuffer> m_CurrentFrameBuffer;


		Ref<Renderbuffer> m_MainRenderbuffer;
		Ref<Texture> m_RenderBufferTexture;

	};

	class Engine
	{
	private:

		static  Unique<EngineSettings> s_Data;

	public:


		static void SetCurrentRenderBuffer(const Ref<Renderbuffer>& framebuffer);
		static  Ref<Renderbuffer>& GetCurrentRenderBuffer();

		static void SetCurrentTexture(const Ref<Texture>& framebuffer);
		static  Ref<Texture>& GetCurrentTexture();
		
		static void SetCurrentFrameBuffer(const Ref<Framebuffer>& framebuffer);
		static  Ref<Framebuffer>& GetCurrentFrameBuffer();

		static void SetMainFramebuffer(const Ref<Framebuffer>& framebuffer);
		static  Ref<Framebuffer>& GetMainFrameBuffer();


		static  Ref<World> CreateWorld(std::string& name);
		
	};
	
}
