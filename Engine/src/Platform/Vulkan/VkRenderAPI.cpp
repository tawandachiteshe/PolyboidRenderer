#include "boidpch.h"
#include "VkRenderAPI.h"

#include <spdlog/spdlog.h>

#include "VkSwapChain.h"
#include "VulkanCommandList.h"
#include "VulkanFramebuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanTexture2D.h"
#include "VulkanVertexBuffer.h"
#include "Utils/VkDebugMessenger.h"
#include "Utils/VkInstance.h"
#include "Utils/VulkanAllocator.h"
#include "Utils/VulkanDescriptorPool.h"
#include "Utils/VulkanDevice.h"
#include "Utils/VulkanPhysicalDevice.h"
#include "Utils/VulkanSurfaceKHR.h"

#define ALLOC_API(Klass, ...) std::make_shared<Klass>(__VA_ARGS__);

namespace Polyboid
{
	Ref<VulkanDevice> VkRenderAPI::GetDevice() const
	{
		return m_Device;
	}

	Ref<VulkanSurfaceKHR> VkRenderAPI::GetSurface() const
	{
		return m_Surface;
	}

	Ref<VulkanSyncObjects> VkRenderAPI::GetSyncObjects() const
	{
		return m_SyncObjects;
	}

	void VkRenderAPI::SetSwapchainIndex(uint32_t index)
	{
		m_SwapchainImageIndex = index;
	}

	VkRenderAPI::VkRenderAPI(const std::any& window): m_Window(window)
	{
		m_Instance = std::make_shared<VkInstance>();
		m_Messenger = std::make_shared<VkDebugMessenger>(m_Instance);
		m_Surface = std::make_shared<VulkanSurfaceKHR>(m_Instance, window);
		m_PhysicalDevice = std::make_shared<VulkanPhysicalDevice>(m_Instance, m_Surface);
		m_Device = std::make_shared<VulkanDevice>(m_PhysicalDevice);

		m_Allocator = std::make_shared<VulkanAllocator>(this);

		m_SyncObjects = ALLOC_API(VulkanSyncObjects, this, MAX_FRAMES_INFLIGHT);
		m_DescPool = ALLOC_API(VulkanDescriptorPool, this);
	}

	void VkRenderAPI::BeginFrame()
	{
		vk::Device device = (*m_Device);


		auto inflightFence = m_SyncObjects->GetFences()[m_CurrentFrame];


		vk::Result result = device.waitForFences(1, &inflightFence, true, std::numeric_limits<uint64_t>::max());

		if (result != vk::Result::eSuccess)
		{
			spdlog::error("Waiting for fence failed");
			__debugbreak();
		}


		for (const auto& swapchain : m_Swapchains)
		{
			swapchain->BeginFrame();
		}

		result = device.resetFences(1, &inflightFence);
		if (result != vk::Result::eSuccess)
		{
			spdlog::error("Reset fence failed");
			__debugbreak();
		}
	}

	void VkRenderAPI::EndFrame()
	{
		if (m_CurrentFrame < MAX_FRAMES_INFLIGHT - 1)
		{
			++m_CurrentFrame;
		}
		else
		{
			m_CurrentFrame = 0;
		}
	}

	Ref<Framebuffer> VkRenderAPI::CreateFrameBuffer(const FramebufferSettings& settings)
	{
		auto framebuffer = ALLOC_API(VulkanFramebuffer, this, settings)

		m_Framebuffers.push_back(framebuffer);

		return framebuffer;
	}

	Ref<Framebuffer> VkRenderAPI::CreateFrameBuffer(const FramebufferSettings& settings,
	                                                const Ref<RenderPass>& renderPass)
	{
		//auto framebuffer = ALLOC_API(VulkanFramebuffer, this, settings, renderPass)

		//m_Framebuffers.push_back(framebuffer);

		return nullptr;
	}

	Ref<Texture> VkRenderAPI::CreateTexture2D(const TextureSettings& settings)
	{
		auto texture = ALLOC_API(VulkanTexture2D, this, settings);
		m_Textures2D.push_back(texture);

		return texture;
	}

	Ref<Texture> VkRenderAPI::CreateTexture2D(const std::any& handle)
	{
		return ALLOC_API(VulkanTexture2D, this, handle);
	}

	Ref<Texture3D> VkRenderAPI::CreateTexture3D(const void** data, const TextureSettings& settings)
	{
		return nullptr;
	}

	Ref<SamplerState> VkRenderAPI::CreateSampler(const SamplerSettings& settings)
	{
		return nullptr;
	}


	Ref<Renderbuffer> VkRenderAPI::CreateRenderBuffer(const RenderbufferSettings& settings)
	{
		return nullptr;
	}

	Ref<UniformBuffer> VkRenderAPI::CreateUniformBuffer(uint32_t size, uint32_t binding)
	{
		return nullptr;
	}

	Ref<IndexBuffer> VkRenderAPI::CreateIndexBuffer(const IndexDataType& type, uint32_t count,
	                                                const std::variant<uint32_t*, uint16_t*>& data)
	{

		auto indexBuffer = ALLOC_API(VulkanIndexBuffer, this, type, data, count);
		m_IndexBuffers.push_back(indexBuffer);

		return indexBuffer;
	}

	Ref<VertexBuffer> VkRenderAPI::CreateVertexBuffer(const void* data, uint32_t dataSize)
	{

		auto vertexBuffer = ALLOC_API(VulkanVertexBuffer, this, data, dataSize);
		m_VertexBuffers.push_back(vertexBuffer);


		return vertexBuffer;
	}

	Ref<VertexBuffer> VkRenderAPI::CreateVertexBuffer(uint32_t dataSize)
	{
		return nullptr;
	}

	Ref<VertexBufferArray> VkRenderAPI::CreateVertexBufferArray()
	{
		return nullptr;
	}

	Ref<CommandList> VkRenderAPI::CreateCommandList()
	{
		auto vulkanCommandList = ALLOC_API(VulkanCommandList, this);

		m_CommandLists.push_back(vulkanCommandList);

		return vulkanCommandList;
	}

	Ref<PipelineState> VkRenderAPI::CreatePipelineState()
	{
		return nullptr;
	}

	Ref<Swapchain> VkRenderAPI::CreateSwapChain(const SwapchainSettings& settings)
	{
		auto swapchain = ALLOC_API(VkSwapChain, this, settings);
		m_Swapchains.push_back(swapchain);

		return swapchain;
	}

	Ref<RenderPass> VkRenderAPI::CreateRenderPass(const RenderPassSettings& settings)
	{
		auto renderPass = ALLOC_API(VulkanRenderPass, this, settings);

		m_RenderPasses.push_back(renderPass);

		return renderPass;
	}


	RenderAPIType VkRenderAPI::GetRenderAPIType()
	{
		return RenderAPIType::Vulkan;
	}

	RenderAPIType VkRenderAPI::GetRenderAPIType() const
	{
		return RenderAPIType::Vulkan;
	}


	VkRenderAPI::~VkRenderAPI()
	{
		const auto instance = m_Instance->GetVKInstance();
		vk::Device device = (*m_Device);
		vk::Result result = device.waitIdle();
		vk::resultCheck(result, "Device failed to wait for idle");

		for (const auto& swapchain : m_Swapchains)
		{
			swapchain->Destroy(device);
		}

		for (const auto& renderPass : m_RenderPasses)
		{
			renderPass->Destroy(device);
		}


		for (const auto& vtx : m_VertexBuffers)
		{
			vtx->Destroy();
		}

		for (const auto& idx : m_IndexBuffers)
		{
			idx->Destroy();
		}

		for (const auto& framebuffer : m_Framebuffers)
		{
			framebuffer->Destroy(device);
		}

		for (const auto& command : m_CommandLists)
		{
			command->Destroy(device);
		}

		m_DescPool->Destroy();

		m_SyncObjects->Destroy(device);
		result = device.waitIdle();
		vk::resultCheck(result, "Device failed to wait for idle");
		m_Allocator->Destroy();
		device.destroy();
		instance.destroySurfaceKHR(*m_Surface);

		instance.destroyDebugUtilsMessengerEXT(m_Messenger->GetMessenger(), nullptr, m_Instance->GetLoader());
		instance.destroy();
	}
}
