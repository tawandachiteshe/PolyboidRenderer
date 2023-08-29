#pragma once
#include <glm/glm.hpp>

#include "Framebuffer.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/Registry/ShaderRegistry.h"


namespace Polyboid
{
	class VertexBuffer;
	class StagingBufferSet;
	class UniformBufferSet;
	class IndexBuffer;
	class VertexBufferSet;
	class GraphicsPipeline;
	class CommandBufferSet;
	class FrameBufferSet;
	class RenderPass;
	class Texture2D;
	class VertexBufferArray;
	class Camera;

	struct CameraBufferData
	{
		glm::mat4 projection;
		glm::mat4 view;
	};

	struct EntityBufferData
	{
		glm::mat4 transform;
		glm::mat4 padding;
	};

	struct Renderer3DStorage
	{
		Ref<Camera> m_CurrentSceneCamera;
		RefPtr<RenderPass> m_CompositeRenderPass;
		RefPtr<FrameBufferSet> m_FrameBuffers;
		Ref<CommandBufferSet> m_CommandBuffer;
		Ref<GraphicsPipeline> m_MainGraphicsPipeline;
		glm::mat4 m_Transform;
		glm::uvec2 m_ViewportSize{0};

		//Camera related stuff
		CameraBufferData m_CameraBuffer;
		Ref<UniformBufferSet> m_CameraUniformBuffer;
		Ref<StagingBufferSet> m_CameraStagingBuffer;
	

		Renderer3DStorage() = default;
	};




	class Renderer3D
	{

	private:
		static GraphicsShaders LoadShader(const std::string& shaderName);
		static ScopePtr<Renderer3DStorage> s_Storage;
	public:

		static void Init(uint32_t width, uint32_t height);

		static void BeginScene(const Ref<Camera>& camera);
		static void DrawMesh(const Ref<VertexBufferSet>& vertexBufferArray, const Ref<IndexBuffer>& indexBuffer, const glm::mat4& transform);
		static void DrawMesh(const Ref<VertexBuffer>& vertexBufferArray, const Ref<IndexBuffer>& indexBuffer, const glm::mat4& transform);
		static void SubmitLights();
		static void Clear(const glm::vec4& color);
		static void EndScene();
		static void ReSize(const glm::uvec2& size);
		static void SetViewport(const glm::vec2& viewport);
		static RefPtr<Texture2D> GetCompositeTexture(const TextureAttachmentSlot& slot, uint32_t frameIndex = 0);
		static  RefPtr<Texture2D> GetCurrentCompositeTexture(const TextureAttachmentSlot& slot);
		static void Shutdown();

	};

}

