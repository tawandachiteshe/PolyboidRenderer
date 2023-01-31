#pragma once
#include "Engine/Engine/Gameplay/World.h"
#include "Engine/Engine/MeshImporter/MeshImporter.h"


namespace Polyboid
{
	class Material;

	struct Plane
	{
		glm::vec3 Normal;
		float distance;
	};

	struct Frustum
	{
		Plane planes[4];
	};

	struct DirectionalLightData
	{
		glm::vec3 Direction;
		glm::vec3 Color;
		float Energy;
	};

	struct PointLightData
	{
		glm::vec3 Position;
		glm::vec3 Color;
		float Distance;
		float Energy;
	};

	struct SpotLightData
	{
		glm::vec3 Position;
		glm::vec3 Direction;
		glm::vec3 Color;

		float InnerAngle;
		float OuterAngle;
		float Distance;
		float Energy;
	};

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
		//Lights
		Ref<ShaderBufferStorage> m_DirectionLightsStorage;
		Ref<ShaderBufferStorage> m_SpotLightsStorage;
		Ref<ShaderBufferStorage> m_PointLightsStorage;
		Ref<ShaderBufferStorage> m_FrustumStorage;

		Ref<ShaderBufferStorage> m_oLightIndexCounterStorage;
		Ref<ShaderBufferStorage> m_tLightIndexCounterStorage;
		Ref<ShaderBufferStorage> m_oLightIndexListStorage;
		Ref<ShaderBufferStorage> m_tLightIndexListStorage;

		//Forward Rendering
		Ref<Framebuffer> m_MainFramebuffer;



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
		Ref<Shader> m_ComputeShader;
		Ref<Shader> m_ForwardRendererShader = nullptr;

		//Deferred Rendering
		Ref<Shader> m_GeompassShader;
		Ref<Shader> m_DepthpassShader;
		Ref<Shader> m_LightpassShader;
		Ref<Shader> m_DeferredRenderer;
		Ref<Shader> m_ComputeFrustumShader;
		Ref<Shader> m_ComputeLightCullingShader;


		//Deferred Rendering...
		//G-Buffer
		Ref<Framebuffer> m_GeomFrameBuffer;
		Ref<Framebuffer> m_LightPassFrameBuffer;
		Ref<Framebuffer> m_Depthpass;


		Ref<Texture3D> m_HDR;
		Ref<Texture3D> m_IrradianceMap;
		Ref<Texture3D> m_PrefilterMap;
		Ref<Texture> m_Hdr2D;
		Ref<Texture> m_BrdfLUT;
		Ref<Texture> m_MainTexture;
		Ref<Texture> m_ComputeTexture;
		Ref<Texture> m_oLightGrid;
		Ref<Texture> m_tLightGrid;

		MeshDataRenderer m_Cube;
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
		void InitLights();

		void PreComputePBRTextures();
		void RenderLights(const Ref<Shader>& shader);
		void RenderMeshes(const Ref<Camera>& camera, const Ref<Shader>& shader);

		void RenderSkybox();

		void Render3D(const Ref<Shader>& shader);
		void Render2D();

	public:
		WorldRenderer(const WorldRendererSettings& settings);
		static Ref<WorldRenderer> Make(const WorldRendererSettings& settings);
		void Render();
	};
}
