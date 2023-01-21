#include "boidpch.h"
#include "WorldRenderer.h"

#include "ComputeRenderer.h"
#include "MaterialLibrary.h"
#include "Primitives.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Engine/Engine/AssetManager.h"
#include "Engine/Engine/Engine.h"


namespace Polyboid
{
	void WorldRenderer::InitFramebuffers()
	{
		// Main Framebuffers
		m_MainFramebuffer = Framebuffer::MakeFramebuffer({1280, 720, {{FramebufferTextureFormat::RGBA8}}});
		m_TestFramebuffer = Framebuffer::MakeFramebuffer({1280, 720});
		m_BugFramebuffer = Framebuffer::MakeFramebuffer({1280, 720});


		FramebufferSettings settings{};
		settings.width = 1280;
		settings.height = 720;
		settings.m_Textures = {
			{FramebufferTextureFormat::RGBA8},
			{FramebufferTextureFormat::RGBA8},
			{FramebufferTextureFormat::RGBA8},
			{FramebufferTextureFormat::RGBA8}
		};


		m_DeferredFramebuffer = Framebuffer::MakeFramebuffer(settings);
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
		m_PBRShader = Shader::MakeShader("Assets/Shaders/renderer3Dpbr.vert", "Assets/Shaders/renderer3Dpbr.frag");
		m_IrradianceShader = Shader::MakeShader("Assets/Shaders/convulateCubemap.vert",
		                                        "Assets/Shaders/convulateCubemap.frag");
		m_PrefilterShader = Shader::MakeShader("Assets/Shaders/prefilter.vert", "Assets/Shaders/prefilter.frag");
		m_BrdfLutShader = Shader::MakeShader("Assets/Shaders/BRDFLut.vert", "Assets/Shaders/BRDFLut.frag");
		m_ShadowMapShader = Shader::MakeShader("Assets/Shaders/shadowMap.vert", "Assets/Shaders/shadowMap.frag");
		m_TexturedQuadShader = Shader::MakeShader("Assets/Shaders/texturedQuad.vert",
		                                          "Assets/Shaders/texturedQuad.frag");
		m_ComputeShader = Shader::MakeShader("Assets/Shaders/TestCompute.comp");
	}

	void WorldRenderer::InitMeshes()
	{
		// Load Meshes
		m_Cube = AssetManager::LoadMesh("Assets/Models/cube.fbx");
		m_Quad = Primitives::GenQuad();
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

	void WorldRenderer::RenderLights()
	{
		auto& registry = m_Settings.world->GetRegistry();

		const auto meshPointLightView = registry.view<TransformComponent, PointLightComponent>();
		const auto meshSpotLightView = registry.view<TransformComponent, SpotLightComponent>();
		const auto meshDirLightView = registry.view<TransformComponent, DirectionLightComponent>();


		int lightCount = 0;

		for (auto entity : meshPointLightView)
		{
			m_RendererShader->Bind();
			m_RendererShader->SetInt("uLightType[" + std::to_string(lightCount) + "]",
			                         static_cast<int>(LightType::Point));
			auto [transform, light] = meshPointLightView.get<TransformComponent, PointLightComponent>(entity);

			m_RendererShader->SetFloat3("uPointLight[" + std::to_string(lightCount) + "].Position", transform.Position);
			m_RendererShader->SetFloat("uPointLight[" + std::to_string(lightCount) + "].Distance", light.Distance);
			m_RendererShader->SetFloat("uPointLight[" + std::to_string(lightCount) + "].Energy", light.Energy);
			m_RendererShader->SetFloat3("uPointLight[" + std::to_string(lightCount) + "].Color", light.color);

			lightCount++;
		}
		m_RendererShader->SetInt("uPointLightsCount", lightCount);

		for (auto entity : meshSpotLightView)
		{
			m_RendererShader->Bind();
			m_RendererShader->SetInt("uLightType[" + std::to_string(lightCount) + "]",
			                         static_cast<int>(LightType::Spot));
			auto [transform, light] = meshSpotLightView.get<TransformComponent, SpotLightComponent>(entity);

			m_RendererShader->SetFloat3("uSpotLight[" + std::to_string(lightCount) + "].Position", transform.Position);
			m_RendererShader->SetFloat3("uSpotLight[" + std::to_string(lightCount) + "].Direction",
			                            glm::normalize(transform.Rotation));
			m_RendererShader->SetFloat("uSpotLight[" + std::to_string(lightCount) + "].Distance", light.Distance);
			m_RendererShader->SetFloat("uSpotLight[" + std::to_string(lightCount) + "].Energy", light.Energy);
			m_RendererShader->SetFloat3("uSpotLight[" + std::to_string(lightCount) + "].Color", light.color);

			m_RendererShader->SetFloat("uSpotLight[" + std::to_string(lightCount) + "].InnerAngle", light.InnerAngle);
			m_RendererShader->SetFloat("uSpotLight[" + std::to_string(lightCount) + "].OuterAngle", light.OuterAngle);

			lightCount++;
		}


		for (auto entity : meshDirLightView)
		{
			m_RendererShader->Bind();
			m_RendererShader->SetInt("uLightType[" + std::to_string(lightCount) + "]",
			                         static_cast<int>(LightType::Directional));
			auto [transform, light] = meshDirLightView.get<TransformComponent, DirectionLightComponent>(entity);

			m_RendererShader->SetFloat3("uDirectionalLight[" + std::to_string(lightCount) + "].Direction",
			                            transform.Rotation);
			m_RendererShader->SetFloat("uDirectionalLight[" + std::to_string(lightCount) + "].Energy", light.Energy);
			m_RendererShader->SetFloat3("uDirectionalLight[" + std::to_string(lightCount) + "].Color", light.color);
		}


		lightCount = 0;
	}

	void WorldRenderer::RenderMeshes(const Ref<Camera>& camera)
	{
		auto& registry = m_Settings.world->GetRegistry();
		const auto meshView = registry.view<TransformComponent, MeshRendererComponent>();

		m_RendererShader->Bind();
		m_RendererShader->SetFloat3("uCameraData.ViewPosition", camera->GetPosition());


		for (auto entity : meshView)
		{
			auto [transform, mesh] = meshView.get<TransformComponent, MeshRendererComponent>(entity);
			auto va = AssetManager::GetMesh(mesh.assetName);
			auto mat = MaterialLibrary::GetMaterial(mesh.materialId);

			m_PrefilterMap->Bind();
			m_BrdfLUT->Bind(1);
			m_IrradianceMap->Bind(2);
			m_HDR->Bind(3);


			Renderer::CullMode(CullMode::Back);
			Renderer::Submit(va, m_RendererShader, transform.GetTransform());
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

	void WorldRenderer::Render3D()
	{
		const auto& camera = GameStatics::GetCurrentCamera();
		//Render skybox
		//temp solution
		Renderer::BeginDraw(camera);
		RenderMeshes(camera);
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
			}
		}

		Renderer::CullMode(CullMode::Front);
		Renderer2D::EndDraw();
	}

	WorldRenderer::WorldRenderer(const WorldRendererSettings& settings): m_Settings(settings)
	{
		InitMeshes();
		InitFramebuffers();
		InitShaders();
		InitRenderbuffers();
		InitTextures();

		PreComputePBRTextures();
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

		RenderLights();
		Render3D();
		Render2D();

		m_MainFramebuffer->UnBind();
	}

	void WorldRenderer::DeferredRenderer()
	{

	}


	void WorldRenderer::Render()
	{
		m_RendererShader = m_PBRShader;

		

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
