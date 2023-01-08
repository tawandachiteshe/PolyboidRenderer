#pragma once
#include "Engine/Engine/Gameplay/World.h"


namespace Polyboid
{
	enum class WorldRendererType
	{
		Forward,
		Deferred
	};

	enum class RendererType
	{
		PBR,
		NON_PBR
	};

	struct WorldRendererSettings
	{
		WorldRendererType type = WorldRendererType::Forward;
		Ref<World> world;

		WorldRendererSettings() = default;
		WorldRendererSettings(const WorldRendererSettings& settings): type(settings.type), world(settings.world) {}
	};

	class WorldRenderer
	{

	private:

		Ref<Framebuffer> m_MainFramebuffer;
		Ref<Framebuffer> m_TestFramebuffer;
		Ref<Framebuffer> m_BugFramebuffer;
		Ref<Framebuffer> m_DeferredFramebuffer;


		Ref<Renderbuffer> m_RenderBuffer;
		Ref<Renderbuffer> m_PrefilterRenderBuffer;
		Ref<Renderbuffer> m_LutRenderBuffer;
		Ref<Renderbuffer> m_IrradianceRenderBuffer;

		Ref<Shader> m_IrradianceShader;
		Ref<Shader> m_PrefilterShader;
		Ref<Shader> m_BrdfLutShader;
		Ref<Shader> m_ShadowMapShader;
		Ref<Shader> m_RenderCubeShader;
		Ref<Shader> m_NonPBRShader;
		Ref<Shader> m_TexturedQuadShader;
		Ref<Shader> m_SkyboxShader;
		Ref<Shader> m_PBRShader;
		Ref<Shader> m_ComputeShader;
		Ref<Shader> m_RendererShader = nullptr;

		Ref<Texture3D> m_HDR;
		Ref<Texture3D> m_IrradianceMap;
		Ref<Texture3D> m_PrefilterMap;
		Ref<Texture> m_Hdr2D;
		Ref<Texture> m_BrdfLUT;
		Ref<Texture> m_MainTexture;
		Ref<Texture> m_ComputeTexture;

		std::vector<Ref<VertexBufferArray>> m_Cube;
		Ref<VertexBufferArray> m_Quad;

	private:
		
		WorldRendererSettings m_Settings;
		void ForwardRenderer();
		void DeferredRenderer();

		void InitFramebuffers();
		void InitRenderbuffers();
		void InitTextures();
		void InitShaders();
		void InitMeshes();

		void PreComputePBRTextures();
		void RenderLights();
		void RenderMeshes(const Ref<Camera>& camera);

		void RenderSkybox();

		void Render3D();
		void Render2D();

	public:
		WorldRenderer(const WorldRendererSettings& settings);
		static Ref<WorldRenderer> Make(const WorldRendererSettings& settings);
		void Render();
	};
}
