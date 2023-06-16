#pragma once

#include "EditorWindow.h"
#include <string>
#include <entt/entt.hpp>

#include "EditorLayer.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "Engine/Engine/Base.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "glm/vec2.hpp"

namespace Polyboid
{
	class MouseScrollEvent;
	class GameObject;
	class EditorCamera;


	class ViewportWindow : public EditorWindow
	{
	
	public:
		ViewportWindow(const std::string& name);
		virtual ~ViewportWindow();
		Ref<EditorCamera> m_ViewportCamera;

		ImVec2 m_LastViewportWindowSize = { 0.0f, 0.0f };
		ImVec2 m_WindowOffset = { 0, 0 };

		Ref<CommandBufferSet> m_EditorCommandBuffer;
		Ref<CommandBufferSet> m_KomputeCommandBuffer;
		Ref<KomputePipeline> m_RefComputePipeline;
		Ref<StorageBufferSet> m_AgeBuffer;
		Ref<GraphicsPipeline> m_Pipeline = nullptr;
		Ref<UniformBufferSet> m_UniformBuffers;
		Ref<StorageBufferSet> m_StorageBuffers;
		Ref<StagingBufferSet> m_UniformStagingBuffers;
		Ref<StagingBufferSet> m_StorageStagingBuffers;
		Ref<StagingBufferSet> m_StorageStagingBuffersVB;
		std::vector<ImTextureID> m_FramebufferTextures;
		Ref<Image2D> m_Image2D;
		Ref<Texture2D> m_ImageTexture;
		Ref<RenderPass> m_RenderPass;
		Ref<FrameBufferSet> m_FrameBuffers;
		Ref<VertexBufferSet> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Vertex m_Vertices[4];
		Vertex m_VerticesBuffer[4];
		CameraBufferData m_CameraData{};
		EntityBufferData m_EntityBufferData{};
		EntityBufferData m_EntityBufferData2{};
		float m_Rotation;

	private:
		GameObject* m_CurrentGameObject = nullptr;
		bool m_Playmode = false;
		bool m_Focused = false;
		bool m_OnResize = false;
		glm::vec2 m_LastViewportSize = {1, 1};
		ImGuizmo::OPERATION m_GizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE m_GizmoMode = ImGuizmo::LOCAL;

		void OnGameObjectSelected(const Event& event);
		void OnGameObjectDeleted(const Event& event);

		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

		void OnEvent(Event& event) override;

	public:
		void OnRender() override;
	};

}
