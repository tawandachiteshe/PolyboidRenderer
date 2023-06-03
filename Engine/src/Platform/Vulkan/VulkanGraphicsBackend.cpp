#include "boidpch.h"
#include "VulkanGraphicsBackend.h"

#include <spdlog/spdlog.h>

#include "VkSwapChain.h"
#include "VulkanCommandBufferSet.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include "VulkanRenderPass.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/ImguiSetup.h"
#include "Engine/Renderer/BufferSet.h"
#include "Engine/Renderer/CommandBufferSet.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RendererSyncObjects.h"
#include "Engine/Renderer/SyncObjects.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{
	void VulkanGraphicsBackend::Init()
	{

		auto vkRenderAPI = dynamic_cast<VkRenderAPI*>(RenderAPI::Get());
		m_GraphicsQueue = vkRenderAPI->GetDevice()->GetGraphicsQueue();
		m_Device = vkRenderAPI->GetDevice()->GetVulkanDevice();

		s_Data = CreateRef<GraphicsBackendData>();
		s_Data->m_Swapchain = Renderer::GetSwapChain();


		s_Data->m_SyncObjects = RendererSyncObjects::Create(Renderer::GetMaxFramesInFlight());
		

		s_Data->m_RenderPasses.reserve(20);
		s_Data->m_Framebuffers.reserve(20);
		s_Data->m_CommandLists.reserve(20);
		s_Data->m_Pipelines.reserve(20);
	}

	void VulkanGraphicsBackend::ClearResources()
	{
		s_Data->m_SubmittingBuffer.clear();
		s_Data->m_RenderPasses.clear();
		s_Data->m_Framebuffers.clear();
		s_Data->m_CommandLists.clear();
		s_Data->m_Pipelines.clear();
	}

	VulkanGraphicsBackend::VulkanGraphicsBackend()
	{
		Init();
	}

	void VulkanGraphicsBackend::SubmitPipeline(const Ref<PipelineState>& pipeline)
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

		if (!m_CanRender) {
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


		for (auto& pool : s_Data->m_CommandLists)
		{
			pool->Recreate();
		}

		for (auto& cmdSet : s_Data->m_CommandBuffers)
		{
			cmdSet.As<VulkanCommandBufferSet>()->Recreate();
		}

		s_Data->m_SubmittingBuffer.clear();

		ClearResources();

		m_SwapchainIndex = 0;
		m_CanRender = true;
	}

	bool VulkanGraphicsBackend::IsReady()
	{
		return m_CanRender;
	}


	void VulkanGraphicsBackend::GetSwapchainImageIndex(uint32_t currentImage)
	{

		auto& syncObjects = s_Data->m_SyncObjects;
		const auto& _imageSemaphore = syncObjects->GetImageSemaphore(currentImage);
		const auto& _fence = syncObjects->GetInFlightFence(currentImage);
		const auto& _renderSemaphore = s_Data->m_SyncObjects->GetRenderSemaphore(currentImage);

		auto fence = std::any_cast<vk::Fence>(_fence->GetHandle());

		auto result = m_Device.waitForFences(fence, VK_TRUE, UINT64_MAX);
		result = m_Device.resetFences({ fence });


		auto imageSemaphore = std::any_cast<vk::Semaphore>(_imageSemaphore->GetHandle());
		vk::Semaphore renderSemaphore = std::any_cast<vk::Semaphore>(_renderSemaphore->GetHandle());
		auto vkSwapchain = std::any_cast<vk::SwapchainKHR>(s_Data->m_Swapchain->GetHandle());


		vk::Device device = VkRenderAPI::GetVulkanDevice();


		vk::Result acquireResult;
		do {
			auto [ImageResult, index] = device.acquireNextImageKHR(vkSwapchain, std::numeric_limits<uint64_t>::max(), imageSemaphore);
			m_SwapchainIndex = index;
			acquireResult = ImageResult;
			if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
				// demo.swapchain is out of date (e.g. the window was resized) and
				// must be recreated:
				Renderer::GetSwapChain()->Resize();
				Imgui::RecreateVulkanRenderer();
				RecreateResources();
				

			}
			else if (acquireResult == vk::Result::eSuboptimalKHR) {
				// swapchain is not as optimal as it could be, but the platform's
				// presentation engine will still present the image correctly.
				break;
			}
			else if (acquireResult == vk::Result::eErrorSurfaceLostKHR) {
				// inst.destroySurfaceKHR(surface);
				// create_surface();
				// resize();
				__debugbreak();
				// surface recreation heer
			}
			else {
				vk::resultCheck(acquireResult, "Failed to acquire image");
			}
		} while (acquireResult != vk::Result::eSuccess);
	}

	void VulkanGraphicsBackend::SubmitGraphicsWork(const std::vector<Ref<CommandBufferSet>>& commandBuffers)
	{
		auto frame = Renderer::GetCurrentFrame();

		s_Data->m_CommandBuffers = commandBuffers;
		auto& syncObjects = s_Data->m_SyncObjects;
		const auto& _imageSemaphore = syncObjects->GetImageSemaphore(frame);
		const auto& _fence = syncObjects->GetInFlightFence(frame);
		const auto& _renderSemaphore = s_Data->m_SyncObjects->GetRenderSemaphore(frame);
		auto vkSwapchain = std::any_cast<vk::SwapchainKHR>(s_Data->m_Swapchain->GetHandle());

		auto imageSemaphore = std::any_cast<vk::Semaphore>(_imageSemaphore->GetHandle());
		vk::Semaphore renderSemaphore = std::any_cast<vk::Semaphore>(_renderSemaphore->GetHandle());
		auto fence = std::any_cast<vk::Fence>(_fence->GetHandle());

		int width = 0, height = 0;
		glfwGetFramebufferSize(std::any_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow()), &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(std::any_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow()), &width, &height);
			glfwWaitEvents();
		}

		const uint32_t imageIndex = m_SwapchainIndex;



		for (const auto& commandSet : commandBuffers)
		{
			vk::CommandBuffer buffer = std::any_cast<vk::CommandBuffer>(commandSet->GetCommandBufferAt(m_SwapchainIndex)->GetHandle());
			s_Data->m_SubmittingBuffer.emplace_back(buffer);
		}


		vk::SubmitInfo submitInfo{};
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &imageSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.setCommandBuffers(s_Data->m_SubmittingBuffer);

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderSemaphore;


	   vk::Result result = m_GraphicsQueue.submit(1, &submitInfo, fence);
		vk::resultCheck(result, "Failed to submit commands");



		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderSemaphore;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pSwapchains = &vkSwapchain;
		presentInfo.swapchainCount = 1;

		result = m_GraphicsQueue.presentKHR(&presentInfo);


		
		const auto& maxFrames = Renderer::GetMaxFramesInFlight();
		frame = (frame + 1) % maxFrames;
		Renderer::SetCurrentFrame(frame);

		if (result == vk::Result::eErrorOutOfDateKHR) {
			// swapchain is out of date (e.g. the window was resized) and
			// must be recreated:
			Renderer::GetSwapChain()->Resize();
			Imgui::RecreateVulkanRenderer();
			for (auto& cmdSet : s_Data->m_CommandBuffers)
			{
				cmdSet.As<VulkanCommandBufferSet>()->Recreate();
			}
			RecreateResources();
		}
		else if (result == vk::Result::eSuboptimalKHR) {
			// SUBOPTIMAL could be due to resize

			vk::SurfaceCapabilitiesKHR surfCapabilities;
			auto surface = VkRenderAPI::GetVulkanSurface();
			auto caps_result = VkRenderAPI::GetVulkanPhysicalDevice().getSurfaceCapabilitiesKHR(surface, &surfCapabilities);
			vk::resultCheck(caps_result, "failed to check");
			if (surfCapabilities.currentExtent.width != width || surfCapabilities.currentExtent.height != height) {
				Renderer::GetSwapChain()->Resize();
				Imgui::RecreateVulkanRenderer();
				RecreateResources();
			}

		}
		else if (result == vk::Result::eErrorSurfaceLostKHR) {
			// inst.destroySurfaceKHR(surface);
			// create_surface();
			// resize();
			__debugbreak();
		}
		else {
			vk::resultCheck(result, "Failed to present");
			
		}

		

		if (m_CanRender)
		{
			ClearResources();
		} 


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

	VulkanGraphicsBackend::~VulkanGraphicsBackend()
	{
	}
}
