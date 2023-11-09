#pragma once
#include <glm/vec3.hpp>

#include "GraphicsBackend.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class KomputePipeline;
	class VulkanComputeBackend;
	class PipelineDescriptorSet;
	class CommandBuffer;
	class CommandBufferSet;

	struct KomputeRenderData
	{

		Ref<CommandBufferSet> m_CurrentCommand;
		Ref<CommandBuffer> m_CommandBuffer;
		Ref<VulkanComputeBackend> m_Backend;
		uint32_t m_CurrentIndex;
		std::vector<Ref<CommandBufferSet>> m_CurrentCommandLists;
	};

	class KomputeCommand
	{

	private:

		static ScopePtr<KomputeRenderData> s_Data;

	public:

		static void Init();

		static void BeginCommands(const Ref<CommandBufferSet>& currentCommand, uint32_t index = 0);
		static void EndCommands();

		static void BeginFrameCommands(const Ref<CommandBufferSet>& currentCommand);
		static void EndFrameCommands();

		static void SetCurrentFrame(uint32_t frameIndex);
		static uint32_t GetCurrentFrame();
		static void BindDescriptorSet(const std::vector<Ref<PipelineDescriptorSet>>& sets, uint32_t setBinding = 0);
		static void Dispatch(const glm::uvec3& workGroup);
		
		static void WaitForWork();


		static void BindKomputePipeline(const Ref<KomputePipeline>& komputePipeline);
		static void PushCommandBufferSet(const Ref<CommandBufferSet>& commandBuffer);
		static void PushCommandBufferSets(const std::vector<Ref<CommandBufferSet>>& commandBuffer);

		static Ref<ComputeBackend> GetComputeBackend();
		static void WaitAndCompute();
		static void ComputeOneTime(const std::vector<Ref<CommandBufferSet>>& commandBuffer, uint32_t frameIndex);

		static Ref<CommandBuffer> GetCurrentCommandBuffer();
		static void SetCurrentCommandBuffer(uint32_t currentIndex);


	};

}

