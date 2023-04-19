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
#include "Engine/Engine/Application.h"
#include "Engine/Renderer/Framebuffer.h"

namespace Polyboid
{

	vk::SwapchainKHR VkSwapChain::s_Swapchain = nullptr;

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
		s_Swapchain = swapchain;

		spdlog::info("Successfully created a swapchain!");


		RenderPassSettings renderPassSettings;
		renderPassSettings.Width = createInfo.imageExtent.width;
		renderPassSettings.Height = createInfo.imageExtent.height;
		renderPassSettings.type = RenderPassType::Present;
		renderPassSettings.TextureAttachments = { { TextureAttachmentSlot::Color0, EngineGraphicsFormats::BGRA8U } };

		m_RenderPass = std::make_shared<VulkanRenderPass>(context, renderPassSettings);
		
	}

	void VkSwapChain::Init()
	{
		Init(m_Context, m_Settings);
	}

	VkSwapChain::VkSwapChain(VkRenderAPI* context, const SwapchainSettings& settings): m_Context(context), m_Settings(settings)
	{

		Init(context, settings);
	}

	std::vector<Ref<VulkanTexture2D>> VkSwapChain::CreateSwapchainTextures(EngineGraphicsFormats imageFormat)
	{

		vk::Format swapchainImageFormat = vk::Format::eB8G8R8A8Unorm;

		switch (imageFormat)
		{
		case EngineGraphicsFormats::BGRA8U:
			swapchainImageFormat = vk::Format::eB8G8R8A8Unorm;
			break;
			default:
				break;
		}
		

		const auto api = dynamic_cast<VkRenderAPI*>(Application::Get().GetRenderAPI());
		const auto engineDevice = api->GetDevice();
		const auto device = engineDevice->GetDevice();


		auto [swapchainImagesResult, swapchainImages] = device.getSwapchainImagesKHR(s_Swapchain);
		vk::resultCheck(swapchainImagesResult, "Failed to get swapchain results");
		std::vector<Ref<VulkanTexture2D>> textures;
		textures.resize(swapchainImages.size());

		//Abstract this to something cool
		int count = 0;
		for (auto& swapchainImage : swapchainImages)
		{
			TextureSettings settings;
			settings.sizedFormat = imageFormat;
			textures[count] = std::make_shared<VulkanTexture2D>(api, settings, swapchainImage);
			count++;
		}

		return textures;

	}

	Ref<RenderPass> VkSwapChain::GetDefaultRenderPass()
	{
		return m_RenderPass;
	}

	void VkSwapChain::BeginFrame()
	{

	

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
		

		if (m_RenderPass)
		{
			m_RenderPass->Destroy(device);
			m_RenderPass = nullptr;
		}
		
	}

	void VkSwapChain::Resize(uint32_t width, uint32_t height)
	{
		m_Resize = true;
	}

	void VkSwapChain::Invalidate()
	{
	}

	void VkSwapChain::SwapBuffers()
	{

	}

	void VkSwapChain::SetVsync(bool vsync)
	{
	}

	VkSwapChain::~VkSwapChain()
	{
		
	}

}
