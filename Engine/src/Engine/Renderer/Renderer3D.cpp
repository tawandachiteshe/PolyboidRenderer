#include "boidpch.h"
#include "Renderer3D.h"

#include "BufferSet.h"
#include "Renderer2D.h"
#include "RenderPass.h"
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

	void Renderer3D::Init()
	{
		RenderPassSettings renderPassSettings{};
		renderPassSettings.Width = Application::Get().GetAppSettings().WindowWidth;
		renderPassSettings.Height = Application::Get().GetAppSettings().WindowHeight;
		renderPassSettings.TextureAttachments = { {TextureAttachmentSlot::Color0, EngineGraphicsFormats::RGBA8} };
		renderPassSettings.debugName = "Offscreen render pass";

		s_Storage->m_CompositeRenderPass = RenderPass::Create(renderPassSettings);
		s_Storage->m_FrameBuffers = FrameBufferSet::Create(s_Storage->m_CompositeRenderPass);

		Renderer2D::Init(s_Storage->m_CompositeRenderPass);
	}

	void Renderer3D::BeginScene(const Ref<Camera>& camera)
	{
	}

	void Renderer3D::SubmitMesh(const Ref<VertexBufferArray>& vertexBufferArray)
	{
	}

	void Renderer3D::SubmitLights()
	{
	}

	void Renderer3D::EndScene()
	{
	}

	RefPtr<Texture2D> Renderer3D::GetCompositeTexture(const TextureAttachmentSlot& slot)
	{
		return s_Storage->m_CompositeRenderPass->GetColorTexture(slot);
	}
}
