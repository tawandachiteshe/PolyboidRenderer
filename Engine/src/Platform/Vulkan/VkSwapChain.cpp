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

#include "VulkanFramebuffer.h"
#include "VulkanTexture2D.h"
#include "Engine/Renderer/Framebuffer.h"

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

		if (m_Textures.size() > 0)
		{
			Destroy(device);
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

		switch (settings.SwapchainFormat)
		{
		case EngineGraphicsFormats::BGRA8ISrgb:

			if (formatMap.contains(vk::Format::eB8G8R8A8Unorm))
			{
				swapChainFormat = vk::Format::eB8G8R8A8Unorm;
				colorSpace = formatMap[vk::Format::eB8G8R8A8Unorm];
			}
			break;
		default:
			spdlog::error("Format Not Supported");
			__debugbreak();
		}


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

		uint32_t imageCount = m_Capabilities.minImageCount + 1;

		vk::SwapchainCreateInfoKHR createInfo{};
		createInfo.sType = vk::StructureType::eSwapchainCreateInfoKHR;
		createInfo.flags = vk::SwapchainCreateFlagsKHR();
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
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


		auto [swapchainImagesResult, swapchainImages] = device.getSwapchainImagesKHR(m_Swapchain);
		vk::resultCheck(swapchainImagesResult, "Failed to get swapchain results");
		m_SwapchainImages = swapchainImages;

		m_SwapChainImageViews.resize(m_SwapchainImages.size());
		m_Textures.resize(m_SwapchainImages.size());
		m_SwapchainBuffers.resize(m_SwapchainImages.size());

		//Abstract this to something cool
		count = 0;
		for (auto& m_SwapchainImage : m_SwapchainImages)
		{

			vk::ImageViewCreateInfo createViewInfo;
			createViewInfo.flags = vk::ImageViewCreateFlags();
			createViewInfo.sType = vk::StructureType::eImageViewCreateInfo;
			createViewInfo.viewType = vk::ImageViewType::e2D;
			createViewInfo.format = swapChainFormat;
			createViewInfo.image = m_SwapchainImage;
			createViewInfo.components = vk::ComponentMapping();
			createViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			createViewInfo.subresourceRange.baseMipLevel = 0;
			createViewInfo.subresourceRange.levelCount = 1;
			createViewInfo.subresourceRange.baseArrayLayer = 0;
			createViewInfo.subresourceRange.layerCount = 1;

			auto [createViewResult, view] = device.createImageView(createViewInfo);
			vk::resultCheck(createViewResult, "Failed to create image view");
			m_SwapChainImageViews[count] = view;

			m_Textures[count] = std::make_shared<VulkanTexture2D>(view);

			count++;
		}

		RenderPassSettings renderPassSettings;
		renderPassSettings.Width = createInfo.imageExtent.width;
		renderPassSettings.Height = createInfo.imageExtent.height;
		renderPassSettings.type = RenderPassType::Present;
		renderPassSettings.TextureAttachments = { { TextureAttachmentSlot::Color0, EngineGraphicsFormats::BGRA8U } };

		m_RenderPass = std::make_shared<VulkanRenderPass>(context, renderPassSettings);
		count = 0;
		for (const auto& texture : m_Textures)
		{
			FramebufferSettings framebufferSettings;
			framebufferSettings.textures = { texture };
			framebufferSettings.height = createInfo.imageExtent.height;
			framebufferSettings.width = createInfo.imageExtent.width;
			framebufferSettings.attachmentSlots = { { TextureAttachmentSlot::Color0  } };


			Ref<VulkanFramebuffer> framebuffer = std::make_shared<VulkanFramebuffer>(context, framebufferSettings, m_RenderPass);
			m_SwapchainBuffers[count] = framebuffer;

			count++;
			
		}
	}

	void VkSwapChain::Init()
	{
		Init(m_Context, m_Settings);
	}

	VkSwapChain::VkSwapChain(VkRenderAPI* context, const SwapchainSettings& settings): m_Context(context), m_Settings(settings)
	{

		Init(context, settings);
	}

	Ref<RenderPass> VkSwapChain::GetDefaultRenderPass()
	{
		return m_RenderPass;
	}

	void VkSwapChain::BeginFrame()
	{

		vk::Device device = *m_Context->GetDevice();
		auto imageSemaphore = m_Context->GetSyncObjects()->GetImageSemaphores()[m_Context->m_CurrentFrame];

		auto result = device.acquireNextImageKHR(m_Swapchain, std::numeric_limits<uint64_t>::max(), imageSemaphore);
		m_Context->m_SwapchainImageIndex = result.value;

		if (result.result == vk::Result::eErrorOutOfDateKHR)
		{
			Init(m_Context, m_Settings);
			return;
		}

		if (result.result != vk::Result::eSuccess || result.result == vk::Result::eSuboptimalKHR)
		{
			spdlog::error("Swapchain not great??");
			__debugbreak();
		}

	}

	void VkSwapChain::Destroy(vk::Device device)
	{
		for (const auto& view : m_SwapChainImageViews)
		{
			device.destroyImageView(view);
		}


		m_SwapChainImageViews.clear();

		
		device.destroySwapchainKHR(m_Swapchain);
		

		m_RenderPass->Destroy(device);
		for (const auto& element : m_SwapchainBuffers)
		{
			element->Destroy(device);
		}

		m_SwapchainBuffers.clear();
		
	}

	void VkSwapChain::Resize(uint32_t width, uint32_t height)
	{
		m_Resize = true;
	}

	void VkSwapChain::SwapBuffers()
	{

		vk::Queue queue = m_Context->GetDevice()->GetPresentQueue();
		auto renderSemaphore = m_Context->GetSyncObjects()->GetRenderSemaphores()[m_Context->m_CurrentFrame];
		uint32_t imageIndex = m_Context->m_SwapchainImageIndex;
		vk::Result presentResult = vk::Result::eSuccess;
	

		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderSemaphore;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pResults = &presentResult;
		presentInfo.swapchainCount = 1;

		auto result = queue.presentKHR(&presentInfo);


		if (presentResult == vk::Result::eErrorOutOfDateKHR){

			Init(m_Context, m_Settings);
		}
		else if (result != vk::Result::eSuccess && result == vk::Result::eSuboptimalKHR)
		{
			spdlog::error("Failed to present");
			__debugbreak();
		}
	}

	void VkSwapChain::SetVsync(bool vsync)
	{
	}

	VkSwapChain::~VkSwapChain()
	{
		
	}

}
