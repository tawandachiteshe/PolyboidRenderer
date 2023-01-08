#include "boidpch.h"
#include "Engine.h"


namespace Polyboid
{
	Unique<EngineSettings> Engine::s_Data = std::make_unique<EngineSettings>();

	void Engine::SetCurrentRenderBuffer(const Ref<Renderbuffer>& framebuffer)
	{
		s_Data->m_MainRenderbuffer = framebuffer;
	}

	Ref<Renderbuffer>& Engine::GetCurrentRenderBuffer()
	{
		return s_Data->m_MainRenderbuffer;
	}

	void Engine::SetCurrentTexture(const Ref<Texture>& framebuffer)
	{
		s_Data->m_RenderBufferTexture = framebuffer;
	}

	Ref<Texture>& Engine::GetCurrentTexture()
	{
		return s_Data->m_RenderBufferTexture;
	}

	void Engine::SetCurrentFrameBuffer(const Ref<Framebuffer>& framebuffer)
	{
		s_Data->m_CurrentFrameBuffer = framebuffer;
	}

	Ref<Framebuffer>& Engine::GetCurrentFrameBuffer()
	{
		return s_Data->m_CurrentFrameBuffer;
	}

	void Engine::SetMainFramebuffer(const Ref<Framebuffer>& framebuffer)
	{
		s_Data->m_MainFrameBuffer = framebuffer;
	}

	Ref<Framebuffer>& Engine::GetMainFrameBuffer()
	{
		return s_Data->m_MainFrameBuffer;
	}

	Ref<World> Engine::CreateWorld(std::string& name)
	{

		auto world = std::make_shared<World>(name);
		s_Data->m_Worlds.emplace_back(world);

		return world;
	}
}
