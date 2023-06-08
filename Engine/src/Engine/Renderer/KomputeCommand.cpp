#include "boidpch.h"
#include "KomputeCommand.h"

#include "CommandBufferSet.h"


namespace Polyboid
{

	ScopePtr<KomputeRenderData> KomputeCommand::s_Data = CreateScope<KomputeRenderData>();


	void KomputeCommand::Init()
	{
		
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

	Ref<CommandBuffer> KomputeCommand::GetCurrentCommandBuffer()
	{
		return s_Data->m_CommandBuffer;
	}

	void KomputeCommand::SetCurrentCommandBuffer(uint32_t currentIndex)
	{
		s_Data->m_CommandBuffer = s_Data->m_CurrentCommand->GetCommandBufferAt(currentIndex);
	}
}
