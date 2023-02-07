#include "boidpch.h"
#include "WorldRenderer.h"

#include "ComputeRenderer.h"
#include "FrustumCulling.h"
#include "MaterialLibrary.h"
#include "PerspectiveCamera.h"
#include "Primitives.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "ShaderBufferStorage.h"
#include "Engine/Engine/AssetManager.h"
#include "Engine/Engine/Engine.h"
#include "glm/gtc/type_ptr.hpp"


namespace Polyboid
{
	void WorldRenderer::InitFramebuffers()
	{
		// Main Framebuffers
		m_MainFramebuffer = Framebuffer::MakeFramebuffer({1280, 720, {{FramebufferTextureFormat::RGBA8}}});


		FramebufferSettings settings{};
		settings.width = 1280;
		settings.height = 720;
		settings.m_Textures = {
			{FramebufferTextureFormat::RGBA8},
			{FramebufferTextureFormat::RGBA32F},
			{FramebufferTextureFormat::RGBA8},
			{FramebufferTextureFormat::RGBA32F},
		};


		m_GeomFrameBuffer = Framebuffer::MakeFramebuffer(settings);
		m_LightPassFrameBuffer = Framebuffer::MakeFramebuffer({1280, 720, {{FramebufferTextureFormat::RGBA8}}});
		m_Depthpass = Framebuffer::MakeFramebuffer({1280, 720});
	}

	void WorldRenderer::InitRenderbuffers()
	{
		uint32_t mapRes = 32;

		//Render buffers
		m_IrradianceRenderBuffer = Renderbuffer::MakeRenderBuffer(mapRes, mapRes);
		m_LutRenderBuffer = Renderbuffer::MakeRenderBuffer(512, 512);
		m_RenderBuffer = Renderbuffer::MakeRenderBuffer(512, 512);
		m_PrefilterRenderBuffer = Renderbuffer::MakeRenderBuffer(512, 512);
	}

	void WorldRenderer::InitTextures()
	{
		uint32_t mapRes = 32;

		// Load Textures
		m_Hdr2D = Texture::MakeTexture2D("Assets/HDRs/thatch_chapel_2k.hdr", true);
		m_HDR = std::make_shared<Texture3D>("Assets/HDRs/thatch_chapel_2k.hdr", 512);
		m_IrradianceMap = std::make_shared<Texture3D>(mapRes);
		m_PrefilterMap = std::make_shared<Texture3D>(512, 6);
		m_BrdfLUT = Texture::MakeTexture2D(512, 512, {TextureInternalFormat::RG16F, ClampToEdge});
		m_ComputeTexture = Texture::MakeTexture2D(512, 512, {TextureInternalFormat::RGBA32F, ClampToEdge});
		m_oPointLightGrid = Texture::MakeTexture2D(80, 45, {TextureInternalFormat::RG32UI, ClampToEdge});
		m_tPointLightGrid = Texture::MakeTexture2D(80, 45, {TextureInternalFormat::RG32UI, ClampToEdge});

		m_oSpotLightGrid = Texture::MakeTexture2D(80, 45, {TextureInternalFormat::RG32UI, ClampToEdge});
		m_tSpotLightGrid = Texture::MakeTexture2D(80, 45, {TextureInternalFormat::RG32UI, ClampToEdge});

		m_oDirLightGrid = Texture::MakeTexture2D(80, 45, {TextureInternalFormat::RG32UI, ClampToEdge});
		m_tDirLightGrid = Texture::MakeTexture2D(80, 45, {TextureInternalFormat::RG32UI, ClampToEdge});

		auto whiteTexture = Texture::MakeTexture2D(1, 1, 4);

		uint32_t data = 0xFFFFFFFF;
		whiteTexture->SetData(&data, sizeof(data));

		AssetManager::LoadTexture(0, whiteTexture);
	}

	void WorldRenderer::InitShaders()
	{
		// Load Shaders
		m_SkyboxShader = Shader::MakeShader("Assets/Shaders/skybox.vert", "Assets/Shaders/skybox.frag");
		m_RenderCubeShader = Shader::MakeShader("Assets/Shaders/renderCube.vert", "Assets/Shaders/renderCube.frag");
		m_NonPBRShader = Shader::MakeShader("Assets/Shaders/renderer3D.vert", "Assets/Shaders/renderer3D.frag");
		m_ForwardRendererShader = Shader::MakeShader("Assets/Shaders/renderer3Dpbr.vert",
		                                             "Assets/Shaders/renderer3Dpbr.frag");
		m_GeompassShader = Shader::MakeShader("Assets/Shaders/renderer3Dpbr.vert", "Assets/Shaders/geomPass.frag");
		m_DepthpassShader = Shader::MakeShader("Assets/Shaders/renderer3Dpbr.vert", "Assets/Shaders/depthPass.frag");
		m_ForwardpassShader = Shader::MakeShader("Assets/Shaders/forwardPass.vert",
		                                         "Assets/Shaders/forwardPass.frag");
		m_LightpassShader = Shader::MakeShader("Assets/Shaders/texturedQuad.vert", "Assets/Shaders/lightPass.frag");


		m_IrradianceShader = Shader::MakeShader("Assets/Shaders/convulateCubemap.vert",
		                                        "Assets/Shaders/convulateCubemap.frag");
		m_PrefilterShader = Shader::MakeShader("Assets/Shaders/prefilter.vert", "Assets/Shaders/prefilter.frag");
		m_BrdfLutShader = Shader::MakeShader("Assets/Shaders/BRDFLut.vert", "Assets/Shaders/BRDFLut.frag");
		m_ShadowMapShader = Shader::MakeShader("Assets/Shaders/shadowMap.vert", "Assets/Shaders/shadowMap.frag");
		m_TexturedQuadShader = Shader::MakeShader("Assets/Shaders/texturedQuad.vert",
		                                          "Assets/Shaders/texturedQuad.frag");
		m_ComputeShader = Shader::MakeShader("Assets/Shaders/TestCompute.comp");
		m_ComputeFrustumShader = Shader::MakeShader("Assets/Shaders/computeFrustrumPlanes.comp");
		m_ComputeLightCullingShader = Shader::MakeShader("Assets/Shaders/lightCulling.comp");
	}

	void WorldRenderer::InitMeshes()
	{
		// Load Meshes
		AssetManager::LoadMesh("Assets/Models/cube.fbx");

		m_Cube = AssetManager::LoadRenderData("cube.fbx");
		m_Quad = Primitives::GenQuad();
	}

	struct LightCount
	{
		uint32_t a;
		uint32_t b;
		uint32_t c;
	};

	void WorldRenderer::InitLights()
	{
		const uint32_t MAX_LIGHTS = 20000;
		m_DirectionLightsStorage = ShaderBufferStorage::Make(sizeof(DirectionalLightData) * MAX_LIGHTS);
		m_SpotLightsStorage = ShaderBufferStorage::Make(sizeof(SpotLightData) * MAX_LIGHTS);
		m_PointLightsStorage = ShaderBufferStorage::Make(sizeof(PointLightData) * MAX_LIGHTS);
		m_FrustumStorage = ShaderBufferStorage::Make(sizeof(Frustum) * 36000);


		uint32_t indexCounter[2]{0, 0};

		m_oLightIndexCounterStorage = ShaderBufferStorage::Make(sizeof(LightCount));
		m_tLightIndexCounterStorage = ShaderBufferStorage::Make(sizeof(LightCount));
		m_oLightIndexListStorage = ShaderBufferStorage::Make(sizeof(LightIndex) * 720000);
		m_tLightIndexListStorage = ShaderBufferStorage::Make(sizeof(LightIndex) * 720000);
	}

	void WorldRenderer::PreComputePBRTextures()
	{
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f,
		                                               0.1f, 1000.0f);

		m_RenderCubeShader->SetMat4("projection", captureProjection);

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


		m_IrradianceRenderBuffer->Bind();
		m_HDR->Bind();
		m_IrradianceShader->Bind();
		m_IrradianceShader->SetMat4("projection", captureProjection);
		for (uint32_t i = 0; i < 6; ++i)
		{
			m_IrradianceShader->SetMat4("view", captureViews[i]);
			m_IrradianceRenderBuffer->WriteTexture3D(i, m_IrradianceMap);
			Renderer::Clear();
			Renderer::CullMode(CullMode::Front);
			Renderer::Submit(m_Cube, m_IrradianceShader);
		}
		m_IrradianceRenderBuffer->UnBind();


		m_PrefilterShader->Bind();
		m_PrefilterShader->SetMat4("projection", captureProjection);
		m_HDR->Bind();
		m_PrefilterRenderBuffer->Bind();
		uint32_t maxMipLevels = 6;
		for (uint32_t mip = 0; mip < maxMipLevels; ++mip)
		{
			uint32_t res = 512u * std::pow(0.5, (double)mip);

			m_PrefilterRenderBuffer->ReSize(res, res);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			m_PrefilterShader->SetFloat("uRoughness", roughness);

			for (uint32_t faceID = 0; faceID < 6; ++faceID)
			{
				m_PrefilterShader->SetMat4("view", captureViews[faceID]);
				m_PrefilterRenderBuffer->WriteTexture3D(faceID, m_PrefilterMap, mip);
				Renderer::Clear();
				Renderer::CullMode(CullMode::Front);
				Renderer::Submit(m_Cube, m_PrefilterShader);
			}
		}
		m_PrefilterRenderBuffer->UnBind();
		m_LutRenderBuffer->Bind();
		m_LutRenderBuffer->WriteTexture2D(m_BrdfLUT);
		Renderer::CullMode(CullMode::Front);
		Renderer::Clear(ClearMode::Depth);
		Renderer::Submit(m_Quad, m_BrdfLutShader);
		m_LutRenderBuffer->UnBind();
	}

	void WorldRenderer::RenderLights(const Ref<Shader>& shader)
	{
		auto& registry = m_Settings.world->GetRegistry();

		const auto meshPointLightView = registry.view<TransformComponent, PointLightComponent>();
		const auto meshSpotLightView = registry.view<TransformComponent, SpotLightComponent>();
		const auto meshDirLightView = registry.view<TransformComponent, DirectionLightComponent>();


		int pointLightCount = 0;
		int pointLightOffset = 0;

		const auto& camera = GameStatics::GetCurrentCamera();



		shader->Bind();


		for (auto entity : meshPointLightView)
		{
			auto [transform, light] = meshPointLightView.get<TransformComponent, PointLightComponent>(entity);


			PointLightData data = {};
			data.Position = transform.Position;
			data.PositionVS = glm::vec3(camera->GetView() * glm::vec4(data.Position, 1.0f));
			data.Color = light.color;
			data.Distance = light.Distance;
			data.Energy = light.Energy;

			m_PointLightsStorage->Bind(2);
			m_PointLightsStorage->SetData(&data, sizeof(data), pointLightOffset);

			pointLightOffset += sizeof(PointLightData);
			pointLightCount++;
		}


		shader->SetInt("uPointLightsCount", pointLightCount);


		int spotLightCount = 0;
		int spotLightOffset = 0;
		for (auto entity : meshSpotLightView)
		{
			auto [transform, light] = meshSpotLightView.get<TransformComponent, SpotLightComponent>(entity);
			SpotLightData data = {};

			data.Position = transform.Position;
			data.PositionVS = glm::vec3(camera->GetView() * glm::vec4(transform.Position, 1.0f));
			data.Direction = glm::normalize(transform.Rotation);
			data.DirectionVS = glm::vec3(camera->GetView() * glm::vec4(data.Direction, 1.0f));
			data.Distance = light.Distance;
			data.Energy = light.Energy;
			data.Color = light.color;
			data.InnerAngle = light.InnerAngle;
			data.OuterAngle = light.OuterAngle;

			m_PointLightsStorage->Bind(3);
			m_SpotLightsStorage->SetData(&data, sizeof(data), spotLightOffset);

			spotLightCount++;
			spotLightOffset += sizeof(SpotLightData);
		}


		shader->SetInt("uSpotLightsCount", spotLightCount);


		int dirLightCount = 0;
		int dirLightOffset = 0;
		for (auto entity : meshDirLightView)
		{
			auto [transform, light] = meshDirLightView.get<TransformComponent, DirectionLightComponent>(entity);

			DirectionalLightData data = {};
			data.Direction = glm::normalize(transform.Rotation);
			data.DirectionVS = glm::vec3(camera->GetView() * glm::vec4(data.Direction, 1.0f));
			data.Energy = light.Energy;
			data.Color = light.color;

			m_PointLightsStorage->Bind(4);
			m_DirectionLightsStorage->SetData(&data, sizeof(data), dirLightOffset);

			dirLightCount++;
			dirLightOffset += sizeof(DirectionalLightData);
		}


		shader->SetInt("uDirectionLightsCount", dirLightCount);


		m_PointLightsStorage->Bind(2);
		m_SpotLightsStorage->Bind(3);
		m_DirectionLightsStorage->Bind(4);
	}

	void WorldRenderer::RenderMeshes(const Ref<Camera>& camera, const Ref<Shader>& shader)
	{
		auto& registry = m_Settings.world->GetRegistry();
		const auto meshView = registry.view<TransformComponent, MeshRendererComponent>();


		for (auto& entity : meshView)
		{
			auto [transform, mesh] = meshView.get<TransformComponent, MeshRendererComponent>(entity);
			auto va = AssetManager::GetRenderData(mesh.assetName);

			m_PrefilterMap->Bind(28);
			m_BrdfLUT->Bind(29);
			m_IrradianceMap->Bind(30);
			m_HDR->Bind(31);


			Renderer::Submit(va, shader, transform.GetTransform());
		}
	}

	void WorldRenderer::RenderSkybox()
	{
		Renderer::DisableDepthMask();
		Renderer::CullMode(CullMode::Front);
		m_BrdfLUT->Bind();
		Renderer::Submit(m_Cube, m_SkyboxShader);
		Renderer::EnableDepthMask();
	}

	void WorldRenderer::Render3D(const Ref<Shader>& shader)
	{
		const auto& camera = GameStatics::GetCurrentCamera();
		//Render skybox
		//temp solution
		Renderer::BeginDraw(camera);
		RenderMeshes(camera, shader);
		Renderer::EndDraw();
	}

	void WorldRenderer::Render2D()
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

	WorldRenderer::WorldRenderer(const WorldRendererSettings& settings): m_Settings(settings)
	{
		InitMeshes();
		InitFramebuffers();
		InitShaders();
		InitRenderbuffers();
		InitTextures();
		InitLights();

		PreComputePBRTextures();

		int workGroupsX = (1280 + (1280 % 16)) / 16;
		int workGroupsY = (720 + (720 % 16)) / 16;

		// for (int i = 0; i < 5 * 16; ++i)
		// {
		// 	for (int j = 0; j < 3 * 16; ++j)
		// 	{
		// 		int count = i + (j * (5 * 16));
		// 		spdlog::info("Index {} x {} y {}", count, i, j);
		// 	}
		// }

		// int c = 0;
		// for (int i = 10; i < 200; i += 256)
		// {
		//
		// 	spdlog::info("I is the word {} {}", i, i / 256);
		// 	c++;
		// }

		glm::uint threadCount = 16 * 16;
		glm::uint passCount = (2 + threadCount - 1) / threadCount;

		glm::uint lightIndex = 0 * threadCount + 2;

		const auto& camera = GameStatics::GetCurrentCamera();


		glm::vec3 viewPos = glm::vec3(camera->GetView() * glm::vec4(glm::vec3(10, 10, 10), 1.0f));

		float d = glm::dot(glm::vec3(0.85, 8.55, -0.52), viewPos);

		bool inSide = d - 0.0f < -20.0f;
	}

	Ref<WorldRenderer> WorldRenderer::Make(const WorldRendererSettings& settings)
	{
		return std::make_shared<WorldRenderer>(settings);
	}

	void WorldRenderer::ForwardRenderer()
	{
		Engine::SetCurrentFrameBuffer(m_MainFramebuffer);

		m_MainFramebuffer->Bind();

		Renderer::Clear();
		Renderer::SetClearColor();

		RenderSkybox();
		RenderLights(m_ForwardRendererShader);

		Renderer::CullMode(CullMode::Back);
		Render3D(m_ForwardRendererShader);
		Render2D();

		m_MainFramebuffer->UnBind();
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

	void WorldRenderer::DeferredRenderer()
	{
		Engine::SetCurrentFrameBuffer(m_MainFramebuffer);
		const auto& camera = GameStatics::GetCurrentCamera();

		m_Depthpass->Bind();
		Renderer::Clear();
		Renderer::CullMode(CullMode::Back);
		Render3D(m_DepthpassShader);
		m_Depthpass->UnBind();

		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		ComputeRenderer::Begin();
		m_ComputeFrustumShader->Bind();
		m_ComputeFrustumShader->SetMat4("uInverseProjection", glm::inverse(camera->GetProjection()));
		m_ComputeFrustumShader->SetFloat2("uScreenDimensions", {
			                                  m_MainFramebuffer->GetSettings().width,
			                                  m_MainFramebuffer->GetSettings().height
		                                  });
		m_FrustumStorage->Bind(0);

		auto threadGroups = TotalNumberOfThreadGroups({1280, 720});

		ComputeRenderer::WriteToBuffer(m_FrustumStorage, m_ComputeFrustumShader, {threadGroups, 1});
		ComputeRenderer::End();

		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		ComputeRenderer::Begin();

		m_Depthpass->BindDepthAttachment();
		m_oPointLightGrid->Bind(1, true);
		m_tPointLightGrid->Bind(2, true);

		m_oSpotLightGrid->Bind(3, true);
		m_tSpotLightGrid->Bind(4, true);

		m_oDirLightGrid->Bind(5, true);
		m_tDirLightGrid->Bind(6, true);
		m_ComputeLightCullingShader->Bind();

		m_ComputeLightCullingShader->SetMat4("uView", camera->GetView());
		m_ComputeLightCullingShader->SetMat4("uInverseProjection", glm::inverse(camera->GetProjection()));
		RenderLights(m_ComputeLightCullingShader);

		m_FrustumStorage->Bind();
		m_PointLightsStorage->Bind(2);
		m_SpotLightsStorage->Bind(3);
		m_DirectionLightsStorage->Bind(4);
		m_oLightIndexCounterStorage->Bind(5);
		m_tLightIndexCounterStorage->Bind(6);
		m_oLightIndexListStorage->Bind(7);
		m_tLightIndexListStorage->Bind(8);

		LightCount data = { 0, 0, 0 };
		m_tLightIndexCounterStorage->SetData(&data, sizeof(data), 0);
		m_oLightIndexCounterStorage->SetData(&data, sizeof(data), 0);

		ComputeRenderer::WriteToBuffer(m_FrustumStorage, m_ComputeLightCullingShader,
		                               {TotalNumberOfThreads({1280, 720}), 1});
		ComputeRenderer::End();

		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		m_MainFramebuffer->Bind();

		Renderer::Clear();
		Renderer::SetClearColor();

		Renderer::CullMode(CullMode::Back);

		m_ForwardpassShader->Bind();

		m_ForwardpassShader->Bind();
		m_PointLightsStorage->Bind(2);
		m_SpotLightsStorage->Bind(3);
		m_DirectionLightsStorage->Bind(4);

		m_tLightIndexListStorage->Bind(5);
		m_tPointLightGrid->Bind(6);
		m_tSpotLightGrid->Bind(7);
		m_tDirLightGrid->Bind(8);

		m_ForwardpassShader->SetFloat2("uScreenDimensions", {
									  m_MainFramebuffer->GetSettings().width,
									  m_MainFramebuffer->GetSettings().height
			});

	
		Render3D(m_ForwardpassShader);
		Render2D();

		m_MainFramebuffer->UnBind();

	}


	void WorldRenderer::Render()
	{
		switch (m_Settings.type)
		{
		case WorldRendererType::Forward:
			ForwardRenderer();
			break;
		case WorldRendererType::Deferred:
			DeferredRenderer();
			break;
		}
	}
}
