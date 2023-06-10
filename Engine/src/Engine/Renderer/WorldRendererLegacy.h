#pragma once

#include <mutex>

#include "Engine/Engine/RenderingData.h"
#include "Engine/Engine/MeshImporter/MeshImporter.h"


namespace Polyboid
{
	class Texture2D;
	class Camera;
	class Texture3D;
	class Shader;
	class Renderbuffer;
	class Framebuffer;
	class World;
	class Material;

	struct LightCount
	{
		uint32_t Point = 0, Spot = 0, Dir = 0;
		LightCount(const LightCount& count) = default;
		LightCount() = default;

		void Reset()
		{
			Point = 0; Spot = 0; Dir = 0;
		}
	};

	struct LightIndex
	{
		uint32_t Point = 0, Spot = 0, Dir = 0;
	};


	struct Plane
	{
		glm::vec3 Normal;
		float distance;
	};

	struct Frustum
	{
		Plane planes[4];
	};


	enum class WorldRendererType
	{
		Forward,
		ForwardPlus
	};

	enum class RendererType
	{
		PBR,
		NON_PBR
	};

	struct ScreenViewBuffer
	{
		glm::mat4 inverseProjection;
		glm::uvec4 tileSizes;
		glm::uvec2 screenSize;
		float scale;
		float bias;
	};

	struct VolumeTileAABB
	{
		glm::vec4 minPoint;
		glm::vec4 maxPoint;
	};

	struct WorldRendererSettings
	{
		WorldRendererType type = WorldRendererType::Forward;
		Ref<World> world;

		WorldRendererSettings() = default;
		WorldRendererSettings(const WorldRendererSettings& settings): type(settings.type), world(settings.world) {}
	};

	class WorldRendererLegacy
	{

	private:

		LightCount m_LightCount = {};

		//Clustered Rendering
		const uint32_t m_GridSizeX = 16;
		const uint32_t m_GridSizeY = 9;
		const uint32_t m_GridSizeZ = 24;
		const uint32_t m_NumClusters = m_GridSizeX * m_GridSizeY * m_GridSizeZ;
		uint32_t m_SizeX = 0, m_SizeY = 0;
		uint32_t m_MaxLightsPerTile = 50;

		ScreenViewBuffer m_ScreenBuffer{};

		//Shaders Storages
		Ref<ShaderBufferStorage> m_FrustumStorage;
		Ref<ShaderBufferStorage> m_LightCountStorage;
		Ref<ShaderBufferStorage> m_tLightGlobalIndexCounter;
		Ref<ShaderBufferStorage> m_oLightGlobalIndexCounter;
		Ref<ShaderBufferStorage> m_tLightIndexList;
		Ref<ShaderBufferStorage> m_oLightIndexList;


		//Lights
		Ref<ShaderBufferStorage> m_DirectionLightsStorage;
		Ref<ShaderBufferStorage> m_SpotLightsStorage;
		Ref<ShaderBufferStorage> m_PointLightsStorage;

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
		Ref<Shader> m_DepthPassShader;
		Ref<Shader> m_ForwardPlusShader;

		//Clustured Tiles
		Ref<Shader> m_ComputeFrustumsShader;
		Ref<Shader> m_LightCullingShader;
		Ref<Shader> m_ClusteredPBRShader;


		//Deferred Rendering...
		//G-Buffer
		Ref<Framebuffer> m_GeomFrameBuffer;
		Ref<Framebuffer> m_LightPassFrameBuffer;
		Ref<Framebuffer> m_Depthpass;


		Ref<Texture3D> m_HDR;
		Ref<Texture3D> m_IrradianceMap;
		Ref<Texture3D> m_PrefilterMap;
		Ref<Texture2D> m_Hdr2D;
		Ref<Texture2D> m_BrdfLUT;
		Ref<Texture2D> m_MainTexture;

		//Texture stuuff

		Ref<Texture2D> m_tPointLightGrid;
		Ref<Texture2D> m_tSpotLightGrid;
		Ref<Texture2D> m_tDirLightGrid;

		Ref<Texture2D> m_oPointLightGrid;
		Ref<Texture2D> m_oSpotLightGrid;
		Ref<Texture2D> m_oDirLightGrid;

		Ref<Texture2D> m_oLightHeatMap;
		Ref<Texture2D> m_tLightHeatMap;


		Ref<VertexBufferArray> m_Quad;

	private:

		WorldRendererSettings m_Settings;
		void ForwardRenderer();
		void ForwardPlusRenderer();

		void ReCalculateFrustums();

		void BindLightsStorage();

		void PushLightData();

		void InitFramebuffers();
		void InitRenderbuffers();
		void InitTextures();
		void InitShaders();
		void InitMeshes();
		void InitLights();
		void InitShaderStorageBuffers();

		void PreComputePBRTextures();
		void RenderLights(const Ref<Shader>& shader);
		void RenderMeshes(const Ref<Camera>& camera, const Ref<Shader>& shader);

		void RenderSkybox();

		void Render3D(const Ref<Shader>& shader);
		void Render2D();

		void InitRenderData();

	public:
		using LightsData = std::tuple<std::vector<PointLightData>, std::vector<SpotLightData>, std::vector<DirectionalLightData>>;
		LightsData m_PointLightData;
		LightCount m_LightCountData = {};

		std::mutex m_PointLightDataMutex;
		std::mutex m_LightDataMutex;

		
		void SubmitLights(const LightsData& data);
		void SubmitLightCount(const LightCount& data);

		WorldRendererLegacy(const WorldRendererSettings& settings);
		static Ref<WorldRendererLegacy> Make(const WorldRendererSettings& settings);
		void Render();
	};
}
