#include "boidpch.h"
#include "VulkanGraphicsBackend.h"

#include <spdlog/spdlog.h>

#include "VkSwapChain.h"
#include "VulkanCommandBufferSet.h"
#include "VulkanComputeBackend.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include "VulkanRenderPass.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/ImguiSetup.h"
#include "Engine/Renderer/BufferSet.h"
#include "Engine/Renderer/CommandBufferSet.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/GraphicsSyncObjects.h"
#include "Engine/Renderer/KomputeCommand.h"
#include "Engine/Renderer/SyncObjects.h"
#include "Utils/VulkanDevice.h"
#include <GLFW/glfw3.h>


namespace Polyboid
{
	void VulkanGraphicsBackend::Init()
	{
		auto vkRenderAPI = dynamic_cast<VkRenderAPI*>(RenderAPI::Get());
		m_GraphicsQueue = vkRenderAPI->GetDevice()->GetGraphicsQueue();
		m_Device = vkRenderAPI->GetDevice()->GetVulkanDevice();

		s_Data = CreateRef<GraphicsBackendData>();
		s_Data->m_Swapchain = RenderCommand::GetSwapChain();


		s_Data->m_SyncObjects = GraphicsSyncObjects::Create();


		s_Data->m_RenderPasses.reserve(20);
		s_Data->m_Framebuffers.reserve(20);
		s_Data->m_CommandBuffers.reserve(20);
		s_Data->m_Pipelines.reserve(20);
	}

	void VulkanGraphicsBackend::ClearResources()
	{
		s_Data->m_CommandBuffers.clear();
		s_Data->m_RenderPasses.clear();
		s_Data->m_Framebuffers.clear();
		s_Data->m_Pipelines.clear();
	}

	VulkanGraphicsBackend::VulkanGraphicsBackend()
	{
		Init();
		m_WaitSemaphores.reserve(2);
	}

	void VulkanGraphicsBackend::SubmitPipeline(const Ref<GraphicsPipeline>& pipeline)
	{
		s_Data->m_Pipelines.emplace_back(pipeline.As<VulkanGraphicsPipeline>());
	}

	void VulkanGraphicsBackend::SubmitRenderpass(const Ref<RenderPass>& renderpass)
	{
		s_Data->m_RenderPasses.emplace_back(renderpass.As<VulkanRenderPass>());
	}

	void VulkanGraphicsBackend::SubmitFramebuffer(const Ref<FrameBufferSet>& framebuffer)
	{
		s_Data->m_Framebuffers.emplace_back(framebuffer);
	}

	void VulkanGraphicsBackend::RecreateResources()
	{
		if (!m_CanRender)
		{
			return;
		}

		m_CanRender = false;

		auto result = m_Device.waitIdle();
		vk::resultCheck(result, "Failed to wait for device");


		for (const auto& renderpass : s_Data->m_RenderPasses)
		{
			renderpass->Recreate();
		}


		for (const auto& framebuffer : s_Data->m_Framebuffers)
		{
			framebuffer->Recreate();
		}

		for (const auto& buffer : s_Data->m_Pipelines)
		{
			buffer->Recreate();
		}


		for (auto& cmdSet : s_Data->m_CommandBuffers)
		{
			cmdSet.As<VulkanCommandBufferSet>()->Recreate();
		}

		for (auto& func : m_FreeFunctions)
		{
			func();
		}


		//s_Data->m_SubmittingBuffer.clear();

		ClearResources();

		m_SwapchainIndex = 0;
		m_CanRender = true;
	}

	void VulkanGraphicsBackend::RegisterResizeFunc(const RenderBackendFreeFunc& freeFunc)
	{
		m_FreeFunctions.emplace_back(freeFunc);
	}

	bool VulkanGraphicsBackend::IsReady()
	{
		return m_CanRender;
	}

	void VulkanGraphicsBackend::Present()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(std::any_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow()), &width,
		                       &height);
		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(std::any_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow()),
			                       &width, &height);
			glfwWaitEvents();
		}

		auto& syncObjects = s_Data->m_SyncObjects;
		const auto& _renderSemaphore = syncObjects->GetRenderSemaphore(RenderCommand::GetCurrentFrame());


		vk::Semaphore renderSemaphore = std::any_cast<vk::Semaphore>(_renderSemaphore->GetHandle());
		auto vkSwapchain = std::any_cast<vk::SwapchainKHR>(s_Data->m_Swapchain->GetHandle());

		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderSemaphore;
		presentInfo.pImageIndices = &m_SwapchainIndex;
		presentInfo.pSwapchains = &vkSwapchain;
		presentInfo.swapchainCount = 1;

		auto result = m_GraphicsQueue.presentKHR(&presentInfo);


		if (result == vk::Result::eErrorOutOfDateKHR)
		{
			// swapchain is out of date (e.g. the window was resized) and
			// must be recreated:
			RenderCommand::GetSwapChain()->Resize();
			Imgui::RecreateVulkanRenderer();
			for (auto& cmdSet : s_Data->m_CommandBuffers)
			{
				cmdSet.As<VulkanCommandBufferSet>()->Recreate();
			}
			RecreateResources();
			ClearResources();
		}
		else if (result == vk::Result::eSuboptimalKHR)
		{
			// SUBOPTIMAL could be due to resize

			vk::SurfaceCapabilitiesKHR surfCapabilities;
			auto surface = VkRenderAPI::GetVulkanSurface();
			auto caps_result = VkRenderAPI::GetVulkanPhysicalDevice().getSurfaceCapabilitiesKHR(
				surface, &surfCapabilities);
			vk::resultCheck(caps_result, "failed to check");
			if (surfCapabilities.currentExtent.width != width || surfCapabilities.currentExtent.height != height)
			{
				RenderCommand::GetSwapChain()->Resize();
				Imgui::RecreateVulkanRenderer();
				RecreateResources();
				ClearResources();
			}
		}
		else if (result == vk::Result::eErrorSurfaceLostKHR)
		{
			// inst.destroySurfaceKHR(surface);
			// create_surface();
			// resize();
			__debugbreak();
		}
		else
		{
			vk::resultCheck(result, "Failed to present");
		}


		if (m_CanRender)
		{
			ClearResources();
		}
	}


	void VulkanGraphicsBackend::GetSwapchainImageIndex(uint32_t currentImage)
	{
		auto& syncObjects = s_Data->m_SyncObjects;
		const auto& _imageSemaphore = syncObjects->GetImageSemaphore(currentImage);
		const auto& _fence = syncObjects->GetInFlightFence(currentImage);
		const auto& _renderSemaphore = s_Data->m_SyncObjects->GetRenderSemaphore(currentImage);

		auto fence = std::any_cast<vk::Fence>(_fence->GetHandle());

		auto result = m_Device.waitForFences(fence, VK_TRUE, UINT64_MAX);
		result = m_Device.resetFences({fence});


		auto imageSemaphore = std::any_cast<vk::Semaphore>(_imageSemaphore->GetHandle());
		vk::Semaphore renderSemaphore = std::any_cast<vk::Semaphore>(_renderSemaphore->GetHandle());
		auto vkSwapchain = std::any_cast<vk::SwapchainKHR>(s_Data->m_Swapchain->GetHandle());


		vk::Device device = VkRenderAPI::GetVulkanDevice();


		vk::Result acquireResult;
		do
		{
			auto [ImageResult, index] = device.acquireNextImageKHR(vkSwapchain, std::numeric_limits<uint64_t>::max(),
			                                                       imageSemaphore);
			m_SwapchainIndex = index;
			acquireResult = ImageResult;
			if (acquireResult == vk::Result::eErrorOutOfDateKHR)
			{
				// demo.swapchain is out of date (e.g. the window was resized) and
				// must be recreated:
				RenderCommand::GetSwapChain()->Resize();
				Imgui::RecreateVulkanRenderer();
				RecreateResources();
				ClearResources();
			}
			else if (acquireResult == vk::Result::eSuboptimalKHR)
			{
				// swapchain is not as optimal as it could be, but the platform's
				// presentation engine will still present the image correctly.
				break;
			}
			else if (acquireResult == vk::Result::eErrorSurfaceLostKHR)
			{
				// inst.destroySurfaceKHR(surface);
				// create_surface();
				// resize();
				__debugbreak();
				// surface recreation heer
			}
			else
			{
				vk::resultCheck(acquireResult, "Failed to acquire image");
			}
		}
		while (acquireResult != vk::Result::eSuccess);
	}

	void VulkanGraphicsBackend::SubmitGraphicsWork(const std::vector<Ref<CommandBufferSet>>& commandBuffers)
	{
		if (!m_CanRender)
		{
			return;
		}

		const auto frame = RenderCommand::GetCurrentFrame();

		s_Data->m_CommandBuffers = commandBuffers;
		const auto& syncObjects = s_Data->m_SyncObjects;
		const auto& _imageSemaphore = syncObjects->GetImageSemaphore(frame);
		const auto& _fence = syncObjects->GetInFlightFence(frame);
		const auto& _renderSemaphore = s_Data->m_SyncObjects->GetRenderSemaphore(frame);

		const auto imageSemaphore = std::any_cast<vk::Semaphore>(_imageSemaphore->GetHandle());
		const vk::Semaphore renderSemaphore = std::any_cast<vk::Semaphore>(_renderSemaphore->GetHandle());
		const auto fence = std::any_cast<vk::Fence>(_fence->GetHandle());


		for (const auto& commandSet : s_Data->m_CommandBuffers)
		{
			vk::CommandBuffer buffer = std::any_cast<vk::CommandBuffer>(
				commandSet->GetCommandBufferAt(m_SwapchainIndex)->GetHandle());
			s_Data->m_SubmittingBuffer.emplace_back(buffer);
		}


		vk::SubmitInfo submitInfo{};
		const vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

		m_WaitSemaphores.emplace_back(imageSemaphore);


		submitInfo.waitSemaphoreCount = static_cast<uint32_t>(m_WaitSemaphores.size());
		submitInfo.pWaitSemaphores = m_WaitSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.setCommandBuffers(s_Data->m_SubmittingBuffer);

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderSemaphore;


		vk::Result result = m_GraphicsQueue.submit(1, &submitInfo, fence);
		vk::resultCheck(result, "Failed to submit commands");

		m_WaitSemaphores.clear();
		s_Data->m_SubmittingBuffer.clear();

		ClearResources();
	}

	void VulkanGraphicsBackend::SubmitOneTimeWork(const Ref<CommandBuffer>& buffer)
	{
		const auto cmdBuffer = std::any_cast<vk::CommandBuffer>(buffer->GetHandle());

		vk::SubmitInfo submitInfo{};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;

		vk::Result result = m_GraphicsQueue.submit(1, &submitInfo, nullptr);
		vk::resultCheck(result, "Failed to submit commands");

		result = m_GraphicsQueue.waitIdle();
		vk::resultCheck(result, "Failed to wait commands");
	}

	uint32_t VulkanGraphicsBackend::GetCurrentImageIndex()
	{
		return m_SwapchainIndex;
	}

	void VulkanGraphicsBackend::WaitForQueue()
	{

		const auto result = m_GraphicsQueue.waitIdle();
		vk::resultCheck(result, "Failed to wait for queue");
	}

	VulkanGraphicsBackend::~VulkanGraphicsBackend()
	{
	}
}
