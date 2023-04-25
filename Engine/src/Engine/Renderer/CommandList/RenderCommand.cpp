#include "boidpch.h"
#include "RenderCommand.h"

#include "Commands/RenderCommands.h"
#include "Engine/Renderer/RenderAPI.h"
#include "Engine/Renderer/PipelineState.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Renderer/CommandList.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/SyncObjects.h"
#include "Platform/Vulkan/VkRenderAPI.h"
#include "Platform/Vulkan/Utils/VulkanDevice.h"

namespace Polyboid
{

	Unique<RenderCommandData> RenderCommand::s_Data = nullptr;

	void RenderCommand::SetCommandLists(const std::vector<Ref<CommandList>>& cmdLists)
	{
		s_Data->m_CommandLists = cmdLists;
	}

	void RenderCommand::Init(RenderAPI* context)
	{
		s_Data = std::make_unique<RenderCommandData>();
		s_Data->m_Context = context;
		s_Data->m_Commands.reserve(2000);
	}

	void RenderCommand::AddCommand(const Ref<Command>& renderCommand)
	{
		for (auto cmdList : s_Data->m_CommandLists)
		{
			auto cmdBuffer = cmdList->GetCommandBufferAt(Renderer::GetCurrentFrame());
			renderCommand->SetContext(cmdBuffer);
			s_Data->m_Commands.emplace_back(renderCommand);
		}
		
	}


	void RenderCommand::WaitAndRender(const Ref<Semaphore>& _imageAvailable, const Ref<Semaphore>& _renderFinished, const Ref<Fence>& inFlight)
	{

		vk::Fence inFlightFence = std::any_cast<vk::Fence>(inFlight->GetHandle());
		vk::Semaphore imageSemaphore = std::any_cast<vk::Semaphore>(_imageAvailable->GetHandle());
		vk::Semaphore renderSemaphore = std::any_cast<vk::Semaphore>(_renderFinished->GetHandle());


		for (const auto& renderCommand : s_Data->m_Commands)
		{
			renderCommand->Execute();
		}

		std::vector<vk::CommandBuffer> m_CommandBuffers;

		for (auto cmdList : s_Data->m_CommandLists)
		{
			auto cmdBuffer = cmdList->GetCommandBufferAt(Renderer::GetCurrentFrame());
			m_CommandBuffers.push_back(std::any_cast<vk::CommandBuffer>(cmdBuffer->GetHandle()));
		}

		auto renderAPI = reinterpret_cast<VkRenderAPI*>(s_Data->m_Context);
		auto gfxQueue = renderAPI->GetDevice()->GetGraphicsQueue();
		vk::SubmitInfo submitInfo{};
		vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &imageSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());
		submitInfo.pCommandBuffers = m_CommandBuffers.data();

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderSemaphore;

		submitInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

	
		vk::Result result = gfxQueue.submit(1, &submitInfo, inFlightFence);
		vk::resultCheck(result, "Failed to submit commands");

		s_Data->m_Commands.clear();

	}
}
