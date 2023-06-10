#include "boidpch.h"
#include "KomputeCommand.h"

#include <spdlog/spdlog.h>

#include "CommandBufferSet.h"
#include "RenderCommand.h"
#include "Platform/Vulkan/VulkanComputeBackend.h"


namespace Polyboid
{

	ScopePtr<KomputeRenderData> KomputeCommand::s_Data = CreateScope<KomputeRenderData>();


	void KomputeCommand::Init()
	{
		s_Data->m_Backend = CreateRef<VulkanComputeBackend>();
	}

	void KomputeCommand::BeginCommands(const Ref<CommandBufferSet>& currentCommand, uint32_t index)
	{
		s_Data->m_CurrentCommand = currentCommand;
		SetCurrentFrame(index);
		SetCurrentCommandBuffer(index);
		GetCurrentCommandBuffer()->Begin();

	}

	void KomputeCommand::SetCurrentFrame(uint32_t frameIndex)
	{
		s_Data->m_CurrentIndex = frameIndex;
	}

	uint32_t KomputeCommand::GetCurrentFrame()
	{
		return s_Data->m_CurrentIndex;
	}

	void KomputeCommand::BindDescriptorSet(const std::vector<Ref<PipelineDescriptorSet>>& sets, uint32_t setBinding)
	{
		GetCurrentCommandBuffer()->BindKomputeDescriptorSet(setBinding, sets.at(GetCurrentFrame()));
	}

	void KomputeCommand::Dispatch(const glm::uvec3& workGroup)
	{
		GetCurrentCommandBuffer()->DispatchKompute(workGroup);
	}

	void KomputeCommand::EndCommands()
	{
		GetCurrentCommandBuffer()->End();
	}

	void KomputeCommand::BeginFrameCommands(const Ref<CommandBufferSet>& currentCommand)
	{
		BeginCommands(currentCommand, RenderCommand::GetCurrentFrame());
	}

	void KomputeCommand::EndFrameCommands()
	{
		EndCommands();
	}

	void KomputeCommand::WaitForWork()
	{
		GetComputeBackend()->WaitAndResetFence();
	}

	void KomputeCommand::BindKomputePipeline(const Ref<KomputePipeline>& komputePipeline)
	{
		GetCurrentCommandBuffer()->BindKomputePipline(komputePipeline);
	}

	void KomputeCommand::PushCommandBufferSet(const Ref<CommandBufferSet>& commandBuffer)
	{
		s_Data->m_CurrentCommandLists.emplace_back(commandBuffer);
	}

	void KomputeCommand::PushCommandBufferSets(const std::vector<Ref<CommandBufferSet>>& commandBuffers)
	{
		s_Data->m_CurrentCommandLists.insert(s_Data->m_CurrentCommandLists.end(), commandBuffers.begin(), commandBuffers.end());
	}

	Ref<ComputeBackend> KomputeCommand::GetComputeBackend()
	{
		return s_Data->m_Backend;
	}

	void KomputeCommand::WaitAndCompute()
	{
		GetComputeBackend()->SubmitComputeWork(s_Data->m_CurrentCommandLists);
	}

	void KomputeCommand::ComputeOneTime(const std::vector<Ref<CommandBufferSet>>& commandBuffer, uint32_t frameIndex)
	{
		GetComputeBackend()->ComputeOneTime(commandBuffer, frameIndex);
	}

	Ref<CommandBuffer> KomputeCommand::GetCurrentCommandBuffer()
	{
		return s_Data->m_CommandBuffer;
	}

	void KomputeCommand::SetCurrentCommandBuffer(uint32_t currentIndex)
	{
		s_Data->m_CommandBuffer = s_Data->m_CurrentCommand->GetCommandBufferAt(currentIndex);
	}
}
