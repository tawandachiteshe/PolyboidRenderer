#include "boidpch.h"
#include "WorldRendererLegacy.h"

#include "ComputeDispatcher.h"
#include "Primitives.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Engine/ECS/Components.h"
#include "glm/gtc/type_ptr.hpp"


namespace Polyboid
{
	void WorldRendererLegacy::InitFramebuffers()
	{
		// Main Framebuffers
		//m_MainFramebuffer = Framebuffer::MakeFramebuffer({1280, 720, {{FramebufferTextureFormat::RGBA8}}});


		// FramebufferSettings settings{};
		// settings.width = 1280;
		// settings.height = 720;
		// settings.m_Textures = {
		// 	{FramebufferTextureFormat::RGBA8},
		// 	{FramebufferTextureFormat::RGBA32F},
		// 	{FramebufferTextureFormat::RGBA8},
		// 	{FramebufferTextureFormat::RGBA32F},
		// };
		//
		//
		// m_GeomFrameBuffer = Framebuffer::MakeFramebuffer(settings);
		// m_LightPassFrameBuffer = Framebuffer::MakeFramebuffer({1280, 720, {{FramebufferTextureFormat::RGBA8}}});
		// m_Depthpass = Framebuffer::MakeFramebuffer({1280, 720});
	}

	void WorldRendererLegacy::InitRenderbuffers()
	{
		uint32_t mapRes = 32;

		//Render buffers
		// m_IrradianceRenderBuffer = Renderbuffer::MakeRenderBuffer(mapRes, mapRes);
		// m_LutRenderBuffer = Renderbuffer::MakeRenderBuffer(512, 512);
		// m_RenderBuffer = Renderbuffer::MakeRenderBuffer(512, 512);
		// m_PrefilterRenderBuffer = Renderbuffer::MakeRenderBuffer(512, 512);
	}

	void WorldRendererLegacy::InitTextures()
	{
		uint32_t mapRes = 32;

		// Load Textures
		// m_Hdr2D = Texture::MakeTexture2D("Assets/HDRs/wasteland_clouds4k.hdr", true);
		// m_HDR = std::make_shared<Texture3D>("Assets/HDRs/wasteland_clouds4k.hdr", 512);
		// m_IrradianceMap = std::make_shared<Texture3D>(mapRes);
		// m_PrefilterMap = std::make_shared<Texture3D>(512, 6);
		// m_BrdfLUT = Texture::MakeTexture2D(512, 512, {TextureInternalFormat::RG16F, ClampToEdge});


		// auto whiteTexture = Texture::MakeTexture2D(1, 1, 4);
		//
		// uint32_t data = 0xFFFFFFFF;
		// whiteTexture->SetData(&data, sizeof(data));
		//
		// AssetManager::LoadTexture(0, whiteTexture);
		//
		// m_tPointLightGrid = Texture::MakeTexture2D(80, 45, { TextureInternalFormat::RG32UI, ClampToEdge });
		// m_tSpotLightGrid = Texture::MakeTexture2D(80, 45, { TextureInternalFormat::RG32UI, ClampToEdge });
		// m_tDirLightGrid = Texture::MakeTexture2D(80, 45, { TextureInternalFormat::RG32UI, ClampToEdge });
		//
		// m_oPointLightGrid = Texture::MakeTexture2D(80, 45, { TextureInternalFormat::RG32UI, ClampToEdge });
		// m_oSpotLightGrid = Texture::MakeTexture2D(80, 45, { TextureInternalFormat::RG32UI, ClampToEdge });
		// m_oDirLightGrid = Texture::MakeTexture2D(80, 45, { TextureInternalFormat::RG32UI, ClampToEdge });
		//
		// m_oLightHeatMap = Texture::MakeTexture2D(1280, 720, { TextureInternalFormat::RGBA32F, ClampToEdge });
		// m_tLightHeatMap = Texture::MakeTexture2D(1280, 720, { TextureInternalFormat::RGBA32F, ClampToEdge });
	}

	void WorldRendererLegacy::InitShaders()
	{
		// Load Shaders
		// m_SkyboxShader = Shader::Create("Assets/Shaders/skybox.vert", "Assets/Shaders/skybox.frag");
		// m_RenderCubeShader = Shader::Create("Assets/Shaders/renderCube.vert", "Assets/Shaders/renderCube.frag");
		// m_NonPBRShader = Shader::Create("Assets/Shaders/renderer3D.vert", "Assets/Shaders/renderer3D.frag");
		// m_ForwardRendererShader = Shader::Create("Assets/Shaders/renderer3Dpbr.vert",
		//                                              "Assets/Shaders/renderer3Dpbr.frag");
		// m_DepthPassShader = Shader::Create("Assets/Shaders/forwardPlus.vert",
		//                                        "Assets/Shaders/depthPass.frag");
		//
		//
		// m_IrradianceShader = Shader::Create("Assets/Shaders/convulateCubemap.vert",
		//                                         "Assets/Shaders/convulateCubemap.frag");
		// m_PrefilterShader = Shader::Create("Assets/Shaders/prefilter.vert", "Assets/Shaders/prefilter.frag");
		// m_BrdfLutShader = Shader::Create("Assets/Shaders/BRDFLut.vert", "Assets/Shaders/BRDFLut.frag");
		// m_ShadowMapShader = Shader::Create("Assets/Shaders/shadowMap.vert", "Assets/Shaders/shadowMap.frag");
		// m_TexturedQuadShader = Shader::Create("Assets/Shaders/texturedQuad.vert",
		//                                           "Assets/Shaders/texturedQuad.frag");
		// m_ComputeShader = Shader::Create("Assets/Shaders/TestCompute.comp");
		// m_ComputeFrustumsShader = Shader::Create("Assets/Shaders/computeFrustrumPlanes.comp");
		// m_LightCullingShader = Shader::Create("Assets/Shaders/lightCulling.comp");
		// m_ClusteredPBRShader = Shader::Create("Assets/Shaders/clusteredPBR.vert",
		//                                           "Assets/Shaders/clusteredPBR.frag");
		// m_ForwardPlusShader = Shader::Create("Assets/Shaders/forwardPlus.vert",
		// 	"Assets/Shaders/forwardPlus.frag");
	}

	void WorldRendererLegacy::InitMeshes()
	{
		// Load Meshes
		// AssetManager::LoadMesh("Assets/Models/cube.fbx");
		//
		// m_Cube = AssetManager::LoadRenderData("cube.fbx");
		// m_Quad = Primitives::GenQuad();
	}

	void WorldRendererLegacy::InitLights()
	{
		// const uint32_t MAX_LIGHTS = 20000;
		// m_DirectionLightsStorage = ShaderBufferStorage::Make(sizeof(DirectionalLightData) * MAX_LIGHTS);
		// m_SpotLightsStorage = ShaderBufferStorage::Make(sizeof(SpotLightData) * MAX_LIGHTS);
		// m_PointLightsStorage = ShaderBufferStorage::Make(sizeof(PointLightData) * MAX_LIGHTS);
	}

	void WorldRendererLegacy::InitShaderStorageBuffers()
	{
		

		// m_LightCountStorage = ShaderBufferStorage::Make(sizeof(LightCount));
		// m_FrustumStorage = ShaderBufferStorage::Make(sizeof(Frustum) * 8000);
		// m_tLightGlobalIndexCounter = ShaderBufferStorage::Make(sizeof(uint32_t) * 3);
		// m_tLightIndexList = ShaderBufferStorage::Make(sizeof(LightIndex) * 720'000);
		//
		// m_oLightGlobalIndexCounter = ShaderBufferStorage::Make(sizeof(uint32_t) * 3);
		// m_oLightIndexList = ShaderBufferStorage::Make(sizeof(LightIndex) * 720'000);
	}



	void WorldRendererLegacy::PreComputePBRTextures()
	{
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f,
		                                               0.1f, 1000.0f);

		//m_RenderCubeShader->SetMat4("projection", captureProjection);

		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
			            glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
			            glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
			            glm::vec3(0.0f, 0.0f, 1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
			            glm::vec3(0.0f, 0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
			            glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
			            glm::vec3(0.0f, -1.0f, 0.0f))
		};


		// m_IrradianceRenderBuffer->Bind();
		// m_HDR->Bind();
		// m_IrradianceShader->Bind();
		// m_IrradianceShader->SetMat4("projection", captureProjection);
		// for (uint32_t i = 0; i < 6; ++i)
		// {
		// 	m_IrradianceShader->SetMat4("view", captureViews[i]);
		// 	m_IrradianceRenderBuffer->WriteTexture3D(i, m_IrradianceMap);
		// 	Renderer::Clear();
		// 	Renderer::CullMode(CullMode::Front);
		// 	Renderer::Submit(m_Cube, m_IrradianceShader);
		// }
		// m_IrradianceRenderBuffer->UnBind();
		//
		//
		// m_PrefilterShader->Bind();
		// m_PrefilterShader->SetMat4("projection", captureProjection);
		// m_HDR->Bind();
		// m_PrefilterRenderBuffer->Bind();
		// uint32_t maxMipLevels = 6;
		// for (uint32_t mip = 0; mip < maxMipLevels; ++mip)
		// {
		// 	uint32_t res = 512u * std::pow(0.5, (double)mip);
		//
		// 	m_PrefilterRenderBuffer->ReSize(res, res);
		//
		// 	float roughness = (float)mip / (float)(maxMipLevels - 1);
		// 	m_PrefilterShader->SetFloat("uRoughness", roughness);
		//
		// 	for (uint32_t faceID = 0; faceID < 6; ++faceID)
		// 	{
		// 		m_PrefilterShader->SetMat4("view", captureViews[faceID]);
		// 		m_PrefilterRenderBuffer->WriteTexture3D(faceID, m_PrefilterMap, mip);
		// 		Renderer::Clear();
		// 		Renderer::CullMode(CullMode::Front);
		// 		Renderer::Submit(m_Cube, m_PrefilterShader);
		// 	}
		// }
		// m_PrefilterRenderBuffer->UnBind();
		// m_LutRenderBuffer->Bind();
		// m_LutRenderBuffer->WriteTexture2D(m_BrdfLUT);
		// Renderer::CullMode(CullMode::Front);
		// Renderer::Clear(ClearMode::Depth);
		// Renderer::Submit(m_Quad, m_BrdfLutShader);
		// m_LutRenderBuffer->UnBind();
	}


	void WorldRendererLegacy::RenderMeshes(const Ref<Camera>& camera, const Ref<Shader>& shader)
	{
		// auto& registry = m_Settings.world->GetRegistry();
		// const auto meshView = registry.view<TransformComponent, MeshRendererComponent>();


		// for (auto& entity : meshView)
		// {
		// 	auto [transform, mesh] = meshView.get<TransformComponent, MeshRendererComponent>(entity);
		// 	auto va = AssetManager::GetRenderData(mesh.assetName);
		//
		// 	m_PrefilterMap->Bind(28);
		// 	m_BrdfLUT->Bind(29);
		// 	m_IrradianceMap->Bind(30);
		// 	m_HDR->Bind(31);
		//
		//
		// 	Renderer::Submit(va, shader, transform.GetTransform());
		// }
	}

	void WorldRendererLegacy::RenderSkybox()
	{
		// Renderer::DisableDepthMask();
		// Renderer::CullMode(CullMode::Front);
		// m_BrdfLUT->Bind();
		// Renderer::Submit(m_Cube, m_SkyboxShader);
		// Renderer::EnableDepthMask();
	}

	void WorldRendererLegacy::Render3D(const Ref<Shader>& shader)
	{
		const auto& camera = GameStatics::GetCurrentCamera();
		//Render skybox
		//temp solution
		Renderer::BeginDraw(camera);
		RenderMeshes(camera, shader);
		Renderer::EndDraw();
	}

	void WorldRendererLegacy::Render2D()
	{
		auto& registry = m_Settings.world->GetRegistry();
		const auto shapeView = registry.view<TransformComponent, ShapeComponent>();

		const auto& camera = GameStatics::GetCurrentCamera();


		Renderer2D::BeginDraw(camera);


		for (const auto entity : shapeView)
		{
			auto [transform, shape] = shapeView.get<TransformComponent, ShapeComponent>(entity);
			switch (shape.type)
			{
			case ShapeType::Quad:
				Renderer2D::DrawQuad(transform.GetTransform(), shape.color);
				break;
			case ShapeType::Circle:
				Renderer2D::DrawCircle(transform.GetTransform(), shape.color, shape.thickness, shape.fade);
				break;
			case ShapeType::Rect:
				Renderer2D::DrawRect(transform.GetTransform(), shape.color);
				break;
			case ShapeType::Cube:
				Renderer2D::DrawCube(transform.GetTransform(), shape.color);
				break;
			case ShapeType::Pyramid:
				Renderer2D::DrawPyramid(transform.GetTransform(), shape.nearPlane, shape.farPlane, shape.distance,
				                        shape.color);
				break;
			}
		}

		Renderer2D::EndDraw();
	}

	void WorldRendererLegacy::InitRenderData()
	{
		InitMeshes();
		InitFramebuffers();
		InitShaders();
		InitRenderbuffers();
		InitTextures();
		InitLights();

		PreComputePBRTextures();
		InitShaderStorageBuffers();

		ReCalculateFrustums();

	}

	void WorldRendererLegacy::SubmitLights(const LightsData& data)
	{
		std::scoped_lock lock(m_PointLightDataMutex);

		m_PointLightData = data;

		//m_PointLightsStorage->SetData(&data, sizeof(data), offset);
	}

	void WorldRendererLegacy::SubmitLightCount(const LightCount& data)
	{
		std::scoped_lock lock(m_LightDataMutex);
		m_LightCountData = data;
		//m_LightCountStorage->SetData(&data, sizeof(data), 0);
	}

	WorldRendererLegacy::WorldRendererLegacy(const WorldRendererSettings& settings): m_Settings(settings)
	{
		// Application::Get().SubmitToRenderThread([&]()
		// {
		// 	InitRenderData();
		// });
	}

	Ref<WorldRendererLegacy> WorldRendererLegacy::Make(const WorldRendererSettings& settings)
	{
		return CreateRef<WorldRendererLegacy>(settings);
	}

	

	void WorldRendererLegacy::ForwardRenderer()
	{
		// Engine::SetCurrentFrameBuffer(m_MainFramebuffer);
		//
		// m_MainFramebuffer->Bind();
		//
		// Renderer::Clear();
		// Renderer::SetClearColor();
		//
		// RenderSkybox();
		//
		// Renderer::CullMode(CullMode::Back);
		// Render3D(m_ForwardRendererShader);
		// Render2D();
		//
		// m_MainFramebuffer->UnBind();
	}

#define BLOCK_SIZE 16

	static glm::ivec2 TotalNumberOfThreads(const glm::ivec2& screenDim)
	{
		return {screenDim.x / BLOCK_SIZE, screenDim.y / BLOCK_SIZE};
	}

	static glm::ivec2 TotalNumberOfThreadGroups(const glm::ivec2& screenDim)
	{
		float y = glm::ceil((float)TotalNumberOfThreads(screenDim).y / (float)BLOCK_SIZE);
		float x = glm::ceil((float)TotalNumberOfThreads(screenDim).x / (float)BLOCK_SIZE);
		return {x, y};
	}



	void WorldRendererLegacy::ForwardPlusRenderer()
	{

		const auto& camera = GameStatics::GetCurrentCamera();

		// //Depth pass here
		// m_Depthpass->Bind();
		//
		// Renderer::BeginDraw(camera);
		// Renderer::Clear();
		// Renderer::SetClearColor();
		//
		// Renderer::CullMode(CullMode::Back);
		// Render3D(m_DepthPassShader);
		//
		// m_Depthpass->UnBind();
		//
		// //Light culling
		// ComputeDispatcher::Begin();
		//
		// m_Depthpass->BindDepthAttachment();
		// m_tPointLightGrid->Bind(1, true);
		// m_tSpotLightGrid->Bind(2, true);
		// m_tDirLightGrid->Bind(3, true);
		//
		// m_oPointLightGrid->Bind(4, true);
		// m_oSpotLightGrid->Bind(5, true);
		// m_oDirLightGrid->Bind(6, true);
		//
		// m_tLightHeatMap->Bind(7, true);
		// m_oLightHeatMap->Bind(8, true);
		//
		// //Storage buffers
		// m_FrustumStorage->Bind(0);
		// m_PointLightsStorage->Bind(1);
		// m_SpotLightsStorage->Bind(2);
		// m_DirectionLightsStorage->Bind(3);
		// m_LightCountStorage->Bind(4);
		// m_oLightGlobalIndexCounter->Bind(5);
		// m_tLightGlobalIndexCounter->Bind(6);
		// m_oLightIndexList->Bind(7);
		// m_tLightIndexList->Bind(8);
		//
		//
		// m_LightCullingShader->Bind();
		// m_LightCullingShader->SetMat4("uInverseProjection", glm::inverse(camera->GetProjection()));
		//
		// ComputeDispatcher::WriteToBuffer(m_LightCullingShader, { TotalNumberOfThreads({1280,720}), 1 });
		//
		// ComputeDispatcher::End();
		//
		//
		// uint32_t data[3] = { 0, 0, 0 };
		// m_tLightGlobalIndexCounter->SetData(data, sizeof(data), 0);
		// m_oLightGlobalIndexCounter->SetData(data, sizeof(data), 0);
		//
		//
		// m_MainFramebuffer->Bind();
		//
		// Renderer::BeginDraw(camera);
		// Renderer::Clear();
		// Renderer::SetClearColor();
		//
		// Renderer::CullMode(CullMode::Back);
		// BindLightsStorage();
		//
		// m_tPointLightGrid->Bind(6);
		// m_tSpotLightGrid->Bind(7);
		// m_tDirLightGrid->Bind(8);
		// m_tLightIndexList->Bind(6);
		//
		// glEnable(GL_DEPTH_TEST);
		// glDepthFunc(GL_LEQUAL);
		// Render3D(m_ForwardPlusShader);
		// glDepthFunc(GL_LESS);
		//
		// // temperate d
		// //glDepthMask(GL_FALSE);
		// //glDepthFunc(GL_LEQUAL);
		//
		// m_MainFramebuffer->UnBind();
		//
		//
		// PushLightData();

	}

	void WorldRendererLegacy::ReCalculateFrustums()
	{

		// const auto& camera = GameStatics::GetCurrentCamera();
		// //Compute Frustums here
		// ComputeDispatcher::Begin();
		//
		// m_FrustumStorage->Bind(0);
		//
		// m_ComputeFrustumsShader->Bind();
		// m_ComputeFrustumsShader->SetMat4("uInverseProjection", glm::inverse(camera->GetProjection()));
		// m_ComputeFrustumsShader->SetFloat2("uScreenDimensions", { 1280, 720 });
		//
		// ComputeDispatcher::WriteToBuffer(m_ComputeFrustumsShader, { TotalNumberOfThreadGroups({ 1280, 720 }), 1 });
		//
		// ComputeDispatcher::End();
	}

	void WorldRendererLegacy::BindLightsStorage()
	{
		// m_PointLightsStorage->Bind(2);
		// m_SpotLightsStorage->Bind(3);
		// m_DirectionLightsStorage->Bind(4);
		// m_LightCountStorage->Bind(5);

	}

	void WorldRendererLegacy::PushLightData()
	{

		auto& [pointData, spotData, dirData] = m_PointLightData;

		// m_PointLightsStorage->SetData(pointData.data(), sizeof(PointLightData) * pointData.size(), 0);
		// m_SpotLightsStorage->SetData(spotData.data(), sizeof(SpotLightData) * spotData.size(), 0);
		// m_DirectionLightsStorage->SetData(dirData.data(), sizeof(DirectionalLightData) * dirData.size(), 0);
		//
		// m_LightCountStorage->SetData(&m_LightCountData, sizeof(m_LightCountData), 0);


		pointData.clear();
		spotData.clear();
		dirData.clear();

		m_LightCountData.Reset();
	}


	void WorldRendererLegacy::Render()
	{
		switch (m_Settings.type)
		{
		case WorldRendererType::Forward:
			ForwardRenderer();
			break;
		case WorldRendererType::ForwardPlus:
			ForwardPlusRenderer();
			break;
		}
	}
}
