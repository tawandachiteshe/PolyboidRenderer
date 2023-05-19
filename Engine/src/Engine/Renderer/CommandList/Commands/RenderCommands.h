#pragma once
#include <glm/vec4.hpp>

#include "imgui.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/LayerContainer.h"
#include "Engine/Renderer/Rect.h"
#include "Engine/Renderer/RenderPass.h"
#include "Engine/Renderer/Viewport.h"


namespace Polyboid
{
	class IndexBuffer;
	class VertexBuffer;
	class PipelineDescriptorSet;
	class CommandList;
	class CommandBuffer;
	enum class PrimitiveType;
	class VertexBufferArray;
	class PipelineState;
	class RenderAPI;

	class Command
	{
	protected:
		Ref<CommandBuffer> m_Context;
		void SetContext(const Ref<CommandBuffer>& context);
	public:

		virtual void Execute() = 0;
		virtual ~Command() = default;

		friend class RenderCommand;
		
	};

	class ClearRenderTargetCommand : public Command
	{
	private:
		Ref<RenderPass> m_State;
		ClearSettings m_ClearSettings;
	public:

		ClearRenderTargetCommand(const Ref<RenderPass>& renderTarget, const ClearSettings& clearSettings);
		void Execute() override;
		~ClearRenderTargetCommand() override = default;
	};

	class SubmitSwapChainCommand : public Command
	{
		Ref<Swapchain> m_Swapchain;

	public:

		SubmitSwapChainCommand(const Ref<Swapchain>& swapchain);
		void Execute() override;
		~SubmitSwapChainCommand() override;
	};

	class DrawIndexedCommand : public Command
	{
	private:
		uint32_t m_Count;
		PrimitiveType m_PrimitiveType;
	public:
		DrawIndexedCommand(uint32_t count, const PrimitiveType& type);
		void Execute() override;
		~DrawIndexedCommand() override;
	};

	class DrawArraysCommand : public Command
	{

	private:
		PrimitiveType m_PrimitiveType;
		uint32_t m_VertexCount;

	public:
		DrawArraysCommand(const PrimitiveType& type, uint32_t vertexCount);
		void Execute() override;
		~DrawArraysCommand() override = default;
	};

	class PipelineStateCommand : public Command
	{
	private:
		Ref<PipelineState> m_State;
	public:

		PipelineStateCommand(const Ref<PipelineState>& pipeLine);
		void Execute() override;
		~PipelineStateCommand() override = default;
	};


	class BeginRenderPassCommand : public Command
	{
	private:
		Ref<RenderPass> m_RenderTarget;
	public:
		BeginRenderPassCommand(const Ref<RenderPass>& renderTarget);
		void Execute() override;
	};

	class BeginSwapChainRenderPass : public Command
	{
	
	public:
		BeginSwapChainRenderPass() = default;
		void Execute() override;
		~BeginSwapChainRenderPass() override = default;
	};

	class EndSwapChainRenderPass : public Command
	{
	public:
		EndSwapChainRenderPass() = default;
		void Execute() override;
		~EndSwapChainRenderPass() override = default;
	};

	class ClearRenderPassCommand : public Command
	{
	private:
		Ref<RenderPass> m_RenderTarget;
		ClearSettings m_Settings;
	public:
		ClearRenderPassCommand(const Ref<RenderPass>& renderTarget, ClearSettings settings);
		void Execute() override;
	};


	class EndRenderPassCommand : public Command
	{
	private:
		Ref<RenderPass> m_RenderTarget;
	public:
		EndRenderPassCommand(const Ref<RenderPass>& renderTarget);
		void Execute() override;
	};

	class BeginImguiRender : public Command
	{
	private:
		Ref<CommandList> m_CmdList;
	public:
		BeginImguiRender(const Ref<CommandList>& cmdList);
		void Execute() override;
	};

	class TestCommand : public Command
	{
	private:
		ImTextureID m_TestDs;
	public:
		explicit TestCommand(ImTextureID ds);
		void Execute() override;
	};


	class RenderImGuiCommand : public Command
	{
	private:
		LayerContainer m_Container;
	public:
		RenderImGuiCommand(const LayerContainer& container);
		void Execute() override;
	};


	class EndImguiRender : public Command
	{
	public:
		EndImguiRender();
		void Execute() override;
	};

	class BeginRenderCommand : public Command
	{
	public:
		void Execute() override;
		~BeginRenderCommand() override = default;
	};

	class RenderImguiCommand : public Command
	{
	public:
		void Execute() override;
		~RenderImguiCommand() override = default;
	};


	class EndRenderCommand : public Command
	{
	public:
		void Execute() override;
		~EndRenderCommand() override = default;
	};


	class PushUniformBufferCommand :public Command
	{
	public:
		Ref<PipelineState> m_State;
		const void* m_Data;
		uint32_t m_DataSize;
		std::string m_Name;

		PushUniformBufferCommand(const Ref<PipelineState>& pipeLine, const std::string& name, const void* data, uint32_t dataSize);

		void Execute() override;
		~PushUniformBufferCommand() override = default;
	};


	class BindGraphicsPipelineCommand : public Command
	{
	public:
		Ref<PipelineState> m_State;
		BindGraphicsPipelineCommand(const Ref<PipelineState>& pipeLine);

		void Execute() override;

		~BindGraphicsPipelineCommand() override = default;
	};

	class BindGraphicsPipelineDescSetsCommand : public Command
	{
	public:
		Ref<PipelineDescriptorSet> m_State;
		uint32_t m_SetBinding = 0;
		BindGraphicsPipelineDescSetsCommand(uint32_t setBinding, const Ref<PipelineDescriptorSet>& pipeLine);

		void Execute() override;

		~BindGraphicsPipelineDescSetsCommand() override = default;
	};


	class BindVertexBufferCommand : public Command
	{
	public:
		Ref<VertexBuffer> m_Buffer;
		BindVertexBufferCommand(const Ref<VertexBuffer>& buffer);

		void Execute() override;

		~BindVertexBufferCommand() override = default;
	};


	class BindIndexBufferCommand : public Command
	{
	public:
		Ref<IndexBuffer> m_Buffer;
		BindIndexBufferCommand(const Ref<IndexBuffer>& buffer);

		void Execute() override;

		~BindIndexBufferCommand() override = default;
	};



	class SetViewportCommand : public Command
	{
	public:
		Viewport m_Viewport{};
		SetViewportCommand(const Viewport& viewport);

		void Execute() override;

		~SetViewportCommand() override = default;
	};

	class SetScissorCommand : public Command
	{
	public:
		Rect m_Rect{};
		SetScissorCommand(const Rect& rect);

		void Execute() override;

		~SetScissorCommand() override = default;
	};


}
