#include "boidpch.h"
#include "VkSwapChain.h"

#include "VkRenderAPI.h"
#include "Utils/VkInstance.h"
#include "Utils/VulkanSurfaceKHR.h"
#include "Utils/VulkanPhysicalDevice.h"
#include "Utils/VulkanDevice.h"
#include <spdlog/spdlog.h>


#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <GLFW/glfw3.h>

#include "VulkanCommandList.h"
#include "VulkanFence.h"
#include "VulkanFramebuffer.h"
#include "VulkanSemaphore.h"
#include "VulkanTexture2D.h"
#include "Engine/Engine/Application.h"
#include "Engine/Renderer/CommandList.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/SyncObjects.h"

namespace Polyboid
{


	VkExtent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
		{
			return capabilities.currentExtent;
		}
		else 
		{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void VkSwapChain::Init(VkRenderAPI* context, const SwapchainSettings& settings)
	{

		

		vk::PhysicalDevice physicalDevice = (*context->GetPhysicalDevice());
		vk::Device device = (*context->GetDevice());
		vk::SurfaceKHR surface = (*context->GetSurface());
		auto indices = context->GetPhysicalDevice()->GetFamilyIndices();
		uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };
		m_PresentQueue = context->GetDevice()->GetGraphicsQueue();

		Destroy(device);



		int width = 0, height = 0;
		glfwGetFramebufferSize(std::any_cast<GLFWwindow*>(m_Context->GetWindow()), &width, &height);
		while (width == 0 || height == 0) {
		 	glfwGetFramebufferSize(std::any_cast<GLFWwindow*>(m_Context->GetWindow()), &width, &height);
			glfwWaitEvents();
		}

		vk::Result result = device.waitIdle();

		if (result != vk::Result::eSuccess)
		{
			spdlog::error("Error failed to wait for device");
			__debugbreak();
		}

		
		auto[capResults, capabilities] =  physicalDevice.getSurfaceCapabilitiesKHR(surface);
		vk::resultCheck(capResults, "Failed to get capabilities");
		m_Capabilities = capabilities;


		 auto [formatsResult, formats] = physicalDevice.getSurfaceFormatsKHR(surface);
		 vk::resultCheck(formatsResult, "Failed to get formats");
		 m_Formats = formats;

		 auto [presentResults, presentModes] = physicalDevice.getSurfacePresentModesKHR(surface);
		 vk::resultCheck(presentResults, "Failed to get present modes");
		 m_PresentModes = presentModes;

		if (m_Formats.empty() && m_PresentModes.empty())
		{
			spdlog::error("Failed to create swapchain!");
			__debugbreak();
		}

		std::map<vk::Format, vk::ColorSpaceKHR> formatMap;
		for (const auto& format : m_Formats)
		{
			formatMap[format.format] = format.colorSpace;
		}


		vk::Format swapChainFormat = vk::Format::eUndefined;
		vk::ColorSpaceKHR colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;

		swapChainFormat = vk::Format::eB8G8R8A8Unorm;
		colorSpace = formatMap.at(vk::Format::eB8G8R8A8Unorm);


		std::map<vk::PresentModeKHR, uint32_t> presentMap;
		uint32_t count = 0;
		for (const auto& presentMode : m_PresentModes)
		{
			presentMap[presentMode] = count;
			count++;
		}

		vk::PresentModeKHR presentMode;

		//TODO: Make this configurable
		if (!presentMap.contains(vk::PresentModeKHR::eMailbox))
		{
			spdlog::error("Present mode not supported");
			__debugbreak();
		}

		presentMode = vk::PresentModeKHR::eMailbox;


		vk::SwapchainCreateInfoKHR createInfo{};
		createInfo.sType = vk::StructureType::eSwapchainCreateInfoKHR;
		createInfo.flags = vk::SwapchainCreateFlagsKHR();
		createInfo.surface = surface;
		createInfo.minImageCount = 3;
		createInfo.imageFormat = swapChainFormat;
		createInfo.imageColorSpace = colorSpace;
		createInfo.imageExtent = chooseSwapExtent(m_Capabilities, std::any_cast<GLFWwindow*>(context->GetWindow()));
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;


		if (indices.GraphicsFamily != indices.PresentFamily)
		{
			createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = vk::SharingMode::eExclusive;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.presentMode = presentMode;
		createInfo.preTransform = m_Capabilities.currentTransform;
		createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		createInfo.clipped = true;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		auto[swapChainResult, swapchain] = device.createSwapchainKHR(createInfo);
		vk::resultCheck(swapChainResult, "Failed to create result");
		m_Swapchain = swapchain;

		spdlog::info("Successfully created a swapchain!");



		m_RenderPass = std::make_shared<VulkanRenderPass>(context, createInfo.imageExtent.width, createInfo.imageExtent.height);

		auto [swapchainImagesResult, swapchainImages] = device.getSwapchainImagesKHR(m_Swapchain);
		vk::resultCheck(swapchainImagesResult, "Failed to get swapchain results");
		std::vector<Ref<VulkanTexture2D>> textures;
		textures.resize(swapchainImages.size());

		//Abstract this to something cool
		for (auto& swapchainImage : swapchainImages)
		{
			m_ColorTextures.push_back(std::make_shared<VulkanTexture2D>(context, swapchainImage));
			TextureSettings textureSettings{};
			textureSettings.Height = createInfo.imageExtent.height;
			textureSettings.Width = createInfo.imageExtent.width;
			textureSettings.usage = ImageUsage::DepthStencilAttachment;
			textureSettings.sizedFormat = EngineGraphicsFormats::Depth32FStencil8;

			auto depthTexture = std::make_shared<VulkanTexture2D>(context, textureSettings);
			m_DepthTextures.push_back(depthTexture);
		}

		
		uint32_t textureCount = 0;
		for (auto& colorTexture : m_ColorTextures)
		{
			FramebufferSettings framebufferSettings;
			framebufferSettings.height = createInfo.imageExtent.height;
			framebufferSettings.width = createInfo.imageExtent.width;
			framebufferSettings.attachmentSlots = { { TextureAttachmentSlot::Color0, EngineGraphicsFormats::BGRA8U } };

			std::vector<Ref<VulkanTexture2D>> attachments;
			attachments.push_back(colorTexture);
			attachments.push_back(m_DepthTextures[textureCount]);

			auto framebuffer = std::make_shared<VulkanFramebuffer>(context, framebufferSettings, m_RenderPass.get(), attachments);
			m_Framebuffers.push_back(framebuffer);
			m_FramebuffersRefs.push_back(framebuffer);

			textureCount++;
		}

		
		
	}

	void VkSwapChain::Init()
	{
		Init(m_Context, m_Settings);
	}

	VkSwapChain::VkSwapChain(VkRenderAPI* context, const SwapchainSettings& settings): m_Context(context),
		m_Settings(settings)
	{

		Init(context, settings);

	}



	vk::SwapchainKHR VkSwapChain::GetSwapChainHandle()
	{
		return m_Swapchain;
	}


	Ref<RenderPass> VkSwapChain::GetDefaultRenderPass()
	{
		return m_RenderPass;
	}


	void VkSwapChain::Destroy(vk::Device device)
	{
		auto result = device.waitIdle();

		vk::resultCheck(result, "Failed to wait");

		if (m_Swapchain)
		{
			device.destroySwapchainKHR(m_Swapchain);
			m_Swapchain = nullptr;
		}

		for (auto& framebuffer : m_Framebuffers)
		{
			framebuffer->Destroy(device);
		}

		if (m_RenderPass)
		{
			m_RenderPass->Destroy(device);
			m_RenderPass = nullptr;
		}

		m_Framebuffers.clear();
		m_FramebuffersRefs.clear();
		m_Formats.clear();
		m_ColorTextures.clear();
		m_DepthTextures.clear();
	}

	void VkSwapChain::Resize(uint32_t width, uint32_t height)
	{
		m_Resize = true;
	}


	void VkSwapChain::Present(const Ref<Semaphore>& _renderSemaphore)
	{

		uint32_t imageIndex = m_SwapchainCurrentImageIndex;

		
		vk::Semaphore renderSemaphore = std::any_cast<vk::Semaphore>(_renderSemaphore->GetHandle());

		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderSemaphore;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.swapchainCount = 1;

		vk::Result result = m_PresentQueue.presentKHR(&presentInfo);

	
		if (result == vk::Result::eErrorOutOfDateKHR) {

			Init(m_Context, m_Settings);
		}
		else if (result != vk::Result::eSuccess && result == vk::Result::eSuboptimalKHR)
		{
			spdlog::error("Failed to present");
			__debugbreak();
		}

	}

	uint32_t VkSwapChain::GetImageIndex(const Ref<Semaphore>& _imageSemaphore)
	{

		auto imageSemaphore = std::any_cast<vk::Semaphore>(_imageSemaphore->GetHandle());


		vk::Device device = (*m_Context->GetDevice());
		auto [ImageResult, index] = device.acquireNextImageKHR(m_Swapchain, std::numeric_limits<uint64_t>::max(), imageSemaphore);


		if (ImageResult != vk::Result::eSuccess)
		{
			__debugbreak();
		}

		m_SwapchainCurrentImageIndex = index;

		vk::resultCheck(ImageResult, "Failed to aquire image");
		return  m_SwapchainCurrentImageIndex;
	}

	uint32_t VkSwapChain::GetCurrentImageIndex()
	{
		return  m_SwapchainCurrentImageIndex;
	}

	std::vector<Ref<Framebuffer>> VkSwapChain::GetFrameBuffers()
	{
		return m_FramebuffersRefs;
	}


	Ref<Framebuffer> VkSwapChain::GetCurrentFrameBuffer()
	{
		return m_Framebuffers.at(m_SwapchainCurrentImageIndex);
	}


	void VkSwapChain::SetVsync(bool vsync)
	{
	}

	VkSwapChain::~VkSwapChain()
	{
		
	}

}
