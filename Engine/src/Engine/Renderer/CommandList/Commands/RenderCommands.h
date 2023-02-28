#pragma once
#include <glm/vec4.hpp>

#include "Engine/Engine/Base.h"
#include "Engine/Renderer/RenderTarget.h"


namespace Polyboid
{
	enum class PrimitiveType;
	class VertexBufferArray;
	class PipelineState;
	class RenderAPI;

	class Command
	{
	protected:
		Ref<RenderAPI> m_Context;
		void SetContext(const Ref<RenderAPI>& context);
	public:

		virtual void Execute() = 0;
		virtual ~Command() = default;

		friend class RenderCommand;
		
	};

	class ClearRenderTargetCommand : public Command
	{
	private:
		Ref<RenderTarget> m_State;
		ClearSettings m_ClearSettings;
	public:

		ClearRenderTargetCommand(const Ref<RenderTarget>& renderTarget, const ClearSettings& clearSettings);
		void Execute() override;
		~ClearRenderTargetCommand() override = default;
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
		Ref<RenderTarget> m_RenderTarget;
	public:
		BeginRenderPassCommand(const Ref<RenderTarget>& renderTarget);
		void Execute() override;
	};


	class EndRenderPassCommand :public Command
	{
	private:
		Ref<RenderTarget> m_RenderTarget;
	public:
		EndRenderPassCommand(const Ref<RenderTarget>& renderTarget);
		void Execute() override;
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

}
