#include "boidpch.h"
#include "Renderer3D.h"

#include "BufferSet.h"
#include "GraphicsPipeline.h"
#include "Material.h"
#include "RenderCommand.h"
#include "Renderer2D.h"
#include "RenderPass.h"
#include "VertexBufferArray.h"
#include "Engine/Engine/Application.h"


namespace Polyboid
{

	ScopePtr<Renderer3DStorage> Renderer3D::s_Storage = CreateScope<Renderer3DStorage>();

	GraphicsShaders Renderer3D::LoadShader(const std::string& shaderName)
	{
		const std::string shaderPath = "Renderer3D/";
		const auto shaderLoadVert = shaderPath + shaderName + ".vert";
		const auto shaderLoadFrag = shaderPath + shaderName + ".frag";

		auto vertShader = ShaderRegistry::Load(shaderLoadVert);
		auto fragShader = ShaderRegistry::Load(shaderLoadFrag);

		return { vertShader, fragShader };
	}

	void Renderer3D::Init(uint32_t width, uint32_t height)
	{
		//Init render pass
		RenderPassSettings renderPassSettings{};
		renderPassSettings.Width = width;
		renderPassSettings.Height = height;
		renderPassSettings.TextureAttachments = { {TextureAttachmentSlot::Color0, EngineGraphicsFormats::RGBA8} };
		renderPassSettings.debugName = "Offscreen render pass";
		renderPassSettings.IsSwapchainRenderPass = false;

		s_Storage->m_ViewportSize = { renderPassSettings.Width, renderPassSettings.Height };
		s_Storage->m_CompositeRenderPass = RenderPass::Create(renderPassSettings);
		s_Storage->m_FrameBuffers = FrameBufferSet::Create(s_Storage->m_CompositeRenderPass);


		//Render commands
		s_Storage->m_CommandBuffer = CommandBufferSet::Create({ 3, CommandType::ManyTime });
		RenderCommand::PushCommandBufferSets({ s_Storage->m_CommandBuffer });

		Renderer2D::Init(s_Storage->m_CompositeRenderPass);

		//Load 3d renderer shaders
		const auto forwardRendererShader = LoadShader("forward_plus");

		//Vertex Buffer layout definition: NOTE: this is forward declaration. Design flaw
		auto vb = VertexBufferSet::Create(1);
		vb->SetLayout({
			{ShaderDataType::Float3, "aPosition"},
			{ShaderDataType::Float4, "aNormal"},
			{ShaderDataType::Float2, "aUV"}
			});
		auto vtxArray = VertexBufferArray::Create();
		vtxArray->AddVertexBufferSet(vb);

		s_Storage->m_MainGraphicsPipeline = GraphicsPipeline::Create();
		s_Storage->m_MainGraphicsPipeline->SetGraphicsShaders(forwardRendererShader);
		s_Storage->m_MainGraphicsPipeline->SetVertexArray(vtxArray);
		s_Storage->m_MainGraphicsPipeline->SetRenderPass(s_Storage->m_CompositeRenderPass);
		s_Storage->m_MainGraphicsPipeline->Bake();





		//Buffers scene related buffers etc. Lights, Camera and other stuff
		s_Storage->m_CameraStagingBuffer = StagingBufferSet::Create(sizeof(CameraBufferData));
		s_Storage->m_CameraUniformBuffer = UniformBufferSet::Create(sizeof(CameraBufferData));


		//Bind resources and write them. Camera resourses and desc set 0
		s_Storage->m_MainGraphicsPipeline->AllocateDescriptorSets();
		s_Storage->m_MainGraphicsPipeline->BindResource("PerSceneData", s_Storage->m_CameraUniformBuffer);
		s_Storage->m_MainGraphicsPipeline->WriteSetResourceBindings();

		//Desc 1 material info
		s_Storage->m_MainGraphicsPipeline->AllocateDescriptorSets(1);

		s_Storage->m_CurrentMaterials.reserve(100);

	}



	void Renderer3D::BeginScene(const Ref<Camera>& camera)
	{
		s_Storage->m_CurrentSceneCamera = camera;

		s_Storage->m_CameraBuffer.view = camera->GetView();
		s_Storage->m_CameraBuffer.projection = camera->GetProjection();

		s_Storage->m_CameraStagingBuffer->SetData(&s_Storage->m_CameraBuffer);

		RenderCommand::BeginFrameCommands(s_Storage->m_CommandBuffer);
		RenderCommand::BeginRenderPass(s_Storage->m_CompositeRenderPass);

		RenderCommand::BindGraphicsPipeline(s_Storage->m_MainGraphicsPipeline);
		RenderCommand::BindGraphicsDescriptorSets(0, (s_Storage->m_MainGraphicsPipeline->GetDescriptorSets(0)));
		//RenderCommand::BindGraphicsDescriptorSets(1, (s_Storage->m_MainGraphicsPipeline->GetDescriptorSets(1)));


		Renderer2D::BeginDraw(camera);


	}

	
	void Renderer3D::EndScene()
	{

		Renderer2D::EndDraw();
		RenderCommand::EndRenderPass();

		s_Storage->m_CameraStagingBuffer->SetData(&s_Storage->m_CameraBuffer);

		RenderCommand::CopyStagingBuffer(s_Storage->m_CameraStagingBuffer, s_Storage->m_CameraUniformBuffer);

		for (const auto& material : s_Storage->m_CurrentMaterials)
		{
			material->UploadMaterial();
		}

		

		Renderer2D::UploadDataToGpu();

		RenderCommand::EndFrameCommands();

		s_Storage->m_CurrentMaterials.clear();

	}

	void Renderer3D::DrawMesh(const Ref<VertexBufferSet>& vertexBufferSet, const Ref<IndexBuffer>& indexBuffer, const glm::mat4& transform)
	{

		EntityBufferData data{};
		data.transform = transform;


		RenderCommand::VertexShaderPushConstants(s_Storage->m_MainGraphicsPipeline, &data, sizeof(data));

		RenderCommand::BindVertexBuffer(vertexBufferSet);
		RenderCommand::BindIndexBuffer(indexBuffer);
		RenderCommand::DrawIndexed(indexBuffer->GetCount());
	}

	void Renderer3D::DrawMesh(const Ref<VertexBuffer>& vertexBufferArray, const Ref<IndexBuffer>& indexBuffer,
		const glm::mat4& transform, const Ref<Material>& material)
	{
		EntityBufferData data{};
		data.transform = transform;

		s_Storage->m_CurrentMaterials.push_back(material);

		RenderCommand::BindGraphicsDescriptorSets(1, material->GetDescSets());

		RenderCommand::VertexShaderPushConstants(s_Storage->m_MainGraphicsPipeline, &data, sizeof(data));

		RenderCommand::BindVertexBuffer(vertexBufferArray);
		RenderCommand::BindIndexBuffer(indexBuffer);
		RenderCommand::DrawIndexed(indexBuffer->GetCount());
	}

	void Renderer3D::SubmitLights()
	{
	}

	void Renderer3D::Clear(const glm::vec4& color)
	{
		RenderCommand::Clear({ color });
	}


	void Renderer3D::ReSize(const glm::uvec2& size)
	{
		if (s_Storage->m_CurrentSceneCamera.Get())
		{
			RenderCommand::WaitForSubmitQueue();
			s_Storage->m_CurrentSceneCamera->SetViewportSize(static_cast<float>(size.x), static_cast<float>(size.y));
			s_Storage->m_CompositeRenderPass->Resize(size.x, size.y);
			s_Storage->m_MainGraphicsPipeline->Recreate();
			s_Storage->m_ViewportSize = size;
		}
	

	}

	void Renderer3D::SetViewport(const glm::vec2& viewportSize)
	{
		Viewport viewport{};
		viewport.Width = viewportSize.x;
		viewport.Height = viewportSize.y;
		viewport.MinDepth = 0.0;
		viewport.MaxDepth = 1.0f;

		RenderCommand::SetViewport(viewport);
		Rect rect{};
		rect.Width = viewportSize.x;
		rect.Height = viewportSize.y;
		RenderCommand::SetScissor(rect);
	}

	RefPtr<Texture2D> Renderer3D::GetCompositeTexture(const TextureAttachmentSlot& slot, uint32_t frameIndex)
	{
		return s_Storage->m_CompositeRenderPass->GetColorTexture(slot, frameIndex);
	}

	RefPtr<Texture2D> Renderer3D::GetCurrentCompositeTexture(const TextureAttachmentSlot& slot)
	{
		return s_Storage->m_CompositeRenderPass->GetColorTexture(slot, RenderCommand::GetCurrentFrame());
	}

	void Renderer3D::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	Ref<Material> Renderer3D::CreateMaterial(const std::string& name)
	{
		return s_Storage->m_MainGraphicsPipeline->CreateMaterial(name);
	}
}
