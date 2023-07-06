#pragma once
#include <glm/glm.hpp>

#include "Framebuffer.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/Registry/ShaderRegistry.h"


namespace Polyboid
{
	class FrameBufferSet;
	class RenderPass;
	class Texture2D;
	class VertexBufferArray;
	class Camera;

	struct Renderer3DStorage
	{
		Ref<Camera> m_CurrentSceneCamera;
		RefPtr<RenderPass> m_CompositeRenderPass;
		RefPtr<FrameBufferSet> m_FrameBuffers;
		glm::mat4 m_Transform;
	

		Renderer3DStorage() = default;
	};

	class Renderer3D
	{

	private:
		static GraphicsShaders LoadShader(const std::string& shaderName);
		static ScopePtr<Renderer3DStorage> s_Storage;
	public:

		static void Init();

		static void BeginScene(const Ref<Camera>& camera);
		static void SubmitMesh(const Ref<VertexBufferArray>& vertexBufferArray);
		static void SubmitLights();
		static void EndScene();
		static RefPtr<Texture2D> GetCompositeTexture(const TextureAttachmentSlot& slot);

	};

}

