#include "boidpch.h"
#include "VkRenderAPI.h"

#include <spdlog/spdlog.h>

#include "Buffers.h"
#include "VkSwapChain.h"
#include "VulkanCommandList.h"
#include "VulkanFence.h"
#include "VulkanFramebuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanIndexBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanSemaphore.h"
#include "VulkanTexture2D.h"
#include "VulkanVertexBuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "VulkanImage2D.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include "VulkanStagingBuffer.h"
#include "Utils/VkDebugMessenger.h"
#include "Utils/VkInstance.h"
#include "Utils/VulkanAllocatorInstance.h"
#include "Utils/VulkanDevice.h"
#include "Utils/VulkanPhysicalDevice.h"
#include "Utils/VulkanSurfaceKHR.h"

#define ALLOC_API(Klass, ...) CreateRef<Klass>(__VA_ARGS__);

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

	vk::Device VkRenderAPI::GetVulkanDevice()
	{
		auto api = reinterpret_cast<VkRenderAPI*>(Get());
		return api->GetDevice()->GetVulkanDevice();
	}

	vk::PhysicalDevice VkRenderAPI::GetVulkanPhysicalDevice()
	{
		auto api = reinterpret_cast<VkRenderAPI*>(Get());
		return api->GetPhysicalDevice()->GetPhysicalDevice();
	}

	vk::Instance VkRenderAPI::GetVulkanInstance()
	{
		auto api = reinterpret_cast<VkRenderAPI*>(Get());
		return api->GetInstance()->GetVKInstance();
	}

	VmaAllocator VkRenderAPI::GetVulkanAllocator()
	{
		auto api = reinterpret_cast<VkRenderAPI*>(Get());
		return *api->GetAllocator();
	}

	void VkRenderAPI::SubmitCommandBuffer(const Ref<CommandBuffer>& cmdBuffer, const Ref<Semaphore>& _imageAvailable, const Ref<Semaphore>& _renderFinished,
	                                      const Ref<Fence>& inFlight)
	{

		vk::Fence inFlightFence = std::any_cast<vk::Fence>(inFlight->GetHandle());
		vk::Semaphore imageSemaphore = std::any_cast<vk::Semaphore>(_imageAvailable->GetHandle());
		vk::Semaphore renderSemaphore = std::any_cast<vk::Semaphore>(_renderFinished->GetHandle());

		vk::CommandBuffer buffer = std::any_cast<vk::CommandBuffer>(cmdBuffer->GetHandle());


		auto gfxQueue = GetDevice()->GetGraphicsQueue();
		vk::SubmitInfo submitInfo{};
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eFragmentShader };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &imageSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;
		
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &buffer;
		
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderSemaphore;


		vk::Result result = gfxQueue.submit(1, &submitInfo, inFlightFence);
		vk::resultCheck(result, "Failed to submit commands");


	}

	void VkRenderAPI::SubmitCommandBuffer(const std::vector<Ref<CommandBuffer>>& cmdBuffers,
		const Ref<Semaphore>& _imageAvailable, const Ref<Semaphore>& _renderFinished, const Ref<Fence>& inFlight)
	{

		vk::Fence inFlightFence = std::any_cast<vk::Fence>(inFlight->GetHandle());
		vk::Semaphore imageSemaphore = std::any_cast<vk::Semaphore>(_imageAvailable->GetHandle());
		vk::Semaphore renderSemaphore = std::any_cast<vk::Semaphore>(_renderFinished->GetHandle());

		for (auto& buffer : cmdBuffers)
		{
			m_CommandBuffersBatching.push_back(std::any_cast<vk::CommandBuffer>(buffer->GetHandle()));
		}
		
		auto gfxQueue = GetDevice()->GetGraphicsQueue();
		vk::SubmitInfo submitInfo{};
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &imageSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.setCommandBuffers(m_CommandBuffersBatching);

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderSemaphore;


		vk::Result result = gfxQueue.submit(1, &submitInfo, inFlightFence);
		vk::resultCheck(result, "Failed to submit commands");

		m_CommandBuffersBatching.clear();

	}

	void VkRenderAPI::SubmitCommandBuffer(const Ref<CommandBuffer>& cmdBuffer)
	{

		std::vector<vk::CommandBuffer> m_CommandBuffers;
		m_CommandBuffers.push_back(std::any_cast<vk::CommandBuffer>(cmdBuffer->GetHandle()));

	
		auto gfxQueue = GetDevice()->GetGraphicsQueue();

		vk::SubmitInfo submitInfo{};
		submitInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());
		submitInfo.pCommandBuffers = m_CommandBuffers.data();

		vk::Result result = gfxQueue.submit(1, &submitInfo, nullptr);
		vk::resultCheck(result, "Failed to submit commands");

		result = gfxQueue.waitIdle();
		vk::resultCheck(result, "Failed to wait commands");
	}


	VkRenderAPI::VkRenderAPI(const std::any& window): m_Window(window)
	{
		m_Instance = CreateRef<VkInstance>();
		m_Messenger = CreateRef<VkDebugMessenger>(m_Instance);
		m_Surface = CreateRef<VulkanSurfaceKHR>(m_Instance, window);
		m_PhysicalDevice = CreateRef<VulkanPhysicalDevice>(m_Instance, m_Surface);
		m_Device = CreateRef<VulkanDevice>(m_PhysicalDevice);
		m_Allocator = CreateRef<VulkanAllocatorInstance>(this);

		m_CommandBuffersBatching.reserve(10);

	}


	Ref<Framebuffer> VkRenderAPI::CreateFrameBuffer(const FramebufferSettings& settings)
	{
		//auto framebuffer = ALLOC_API(VulkanFramebuffer, this, settings)

		//m_Framebuffers.push_back(framebuffer);

		__debugbreak();

		return nullptr;
	}

	Ref<Framebuffer> VkRenderAPI::CreateFrameBuffer(const Ref<RenderPass>& renderPass)
	{
		auto framebuffer = ALLOC_API(VulkanFramebuffer, this, renderPass.As<VulkanRenderPass>());

		m_Framebuffers.push_back(framebuffer);

		return framebuffer.As<Framebuffer>();
	}

	Ref<Texture> VkRenderAPI::CreateTexture2D(const TextureSettings& settings, const void* data)
	{
		auto texture = ALLOC_API(VulkanTexture2D, this, settings, data);
		//m_Textures2D.push_back(texture);

		return texture.As<Texture>();
	}

	Ref<Texture> VkRenderAPI::CreateTexture2D(const std::any& handle)
	{
		return nullptr;
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
		auto buffer = ALLOC_API(VulkanUniformBuffer, this, size, binding);
		m_UniformBuffers.emplace_back(buffer);
		return buffer.As<UniformBuffer>();
	}

	Ref<StorageBuffer> VkRenderAPI::CreateStorageBuffer(uint32_t size)
	{
		auto buffer = ALLOC_API(VulkanShaderStorage,this, size);
		m_StorageBuffers.emplace_back(buffer);
		return buffer.As<StorageBuffer>();
	}

	Ref<IndexBuffer> VkRenderAPI::CreateIndexBuffer(const IndexDataType& type, uint32_t count,
	                                                const std::variant<uint32_t*, uint16_t*>& data)
	{

		auto indexBuffer = ALLOC_API(VulkanIndexBuffer, this, type, data, count);
		m_IndexBuffers.push_back(indexBuffer);


		return indexBuffer.As<IndexBuffer>();
	}

	Ref<VertexBuffer> VkRenderAPI::CreateVertexBuffer(const void* data, uint32_t dataSize)
	{

		auto vertexBuffer = ALLOC_API(VulkanVertexBuffer, this, data, dataSize);
		m_VertexBuffers.push_back(vertexBuffer);


		return vertexBuffer.As<VertexBuffer>();
	}

	Ref<VertexBuffer> VkRenderAPI::CreateVertexBuffer(uint32_t dataSize)
	{
		return nullptr;
	}

	Ref<VertexBufferArray> VkRenderAPI::CreateVertexBufferArray()
	{

		auto vaBuffer = ALLOC_API(VulkanVertexBufferArray);

		return vaBuffer.As<VertexBufferArray>();
	}

	Ref<CommandList> VkRenderAPI::CreateCommandList(const CommandListSettings& settings)
	{
		auto vulkanCommandList = ALLOC_API(VulkanCommandList, this, settings);

		m_CommandLists.push_back(vulkanCommandList);

		return vulkanCommandList.As<CommandList>();
	}

	Ref<StagingBuffer> VkRenderAPI::CreateStagingBuffer(uint32_t size)
	{
		auto buffer = ALLOC_API(VulkanStagingBuffer, this, size);
		m_StagingBuffers.emplace_back(buffer);
		return buffer.As<StagingBuffer>();
	}

	Ref<PipelineState> VkRenderAPI::CreatePipelineState()
	{
		auto graphicsPipeline = ALLOC_API(VulkanGraphicsPipeline, this);

		m_Pipelines.push_back(graphicsPipeline);

		return graphicsPipeline.As<PipelineState>();
	}

	Ref<Swapchain> VkRenderAPI::CreateSwapChain(const SwapchainSettings& settings)
	{
		auto swapchain = ALLOC_API(VkSwapChain, this, settings);
		m_Swapchains.push_back(swapchain);

		return swapchain.As<Swapchain>();
	}

	Ref<RenderPass> VkRenderAPI::CreateRenderPass(const RenderPassSettings& settings)
	{
		auto renderPass = ALLOC_API(VulkanRenderPass, this, settings);

		m_RenderPasses.push_back(renderPass);

		return renderPass.As<RenderPass>();
	}

	Ref<Fence> VkRenderAPI::CreateGraphicsFence()
	{
		auto fence = ALLOC_API(VulkanFence, this);

		m_Fences.push_back(fence);

		return fence.As<Fence>();

	}

	Ref<Semaphore> VkRenderAPI::CreateGraphicsSemaphore()
	{

		auto semaphore = ALLOC_API(VulkanSemaphore, this);
		m_Semaphores.push_back(semaphore);

	
		return semaphore.As<Semaphore>();
	}

	Ref<Image2D> VkRenderAPI::CreateImage2D(const ImageSettings& imageSettings)
	{
		auto image2D = ALLOC_API(VulkanImage2D, this, imageSettings);
		m_Image2Ds.push_back(image2D);

		return image2D.As<Image2D>();
	}

	Ref<Shader> VkRenderAPI::CreateShader(const ShaderBinaryAndReflectionInfo& info)
	{
		auto shader = ALLOC_API(VulkanShader, this, info);
		m_VulkanShaders.push_back(shader);
		return shader.As<Shader>();
	}

	Ref<PipelineDescriptorSetPool> VkRenderAPI::CreateDescriptorSetPool(const DescriptorSetPoolSettings& settings)
	{
		auto pool = ALLOC_API(VulkanPipelineDescriptorSetPool, this, settings);
		m_DescPools.emplace_back(pool);
		return pool.As<PipelineDescriptorSetPool>();
	}

	void VkRenderAPI::WaitForFences(const Ref<Fence>& fence)
	{

		auto vulkanFence = std::any_cast<vk::Fence>(fence->GetHandle());
		vk::Result result = GetDevice()->GetVulkanDevice().waitForFences(1, &vulkanFence, true, std::numeric_limits<uint64_t>::max());
		vk::resultCheck(result, "Failed to wait for fence");

		result = GetDevice()->GetVulkanDevice().resetFences(1, &vulkanFence);
		vk::resultCheck(result, "Failed to wait for fence");
	}

	void VkRenderAPI::Destroy()
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

		for (auto& fence : m_Fences)
		{
			fence->Destroy();
		}

		for (auto& semaphore : m_Semaphores)
		{
			semaphore->Destroy();
		}

		for (auto& image : m_Image2Ds)
		{
			image->Destroy();
		}

		for (const auto& vtx : m_VertexBuffers)
		{
			vtx->Destroy();
		}

		// for (const auto& texture : m_Textures2D)
		// {
		// 	texture->Destroy();
		// }

		for (const auto& idx : m_IndexBuffers)
		{
			idx->Destroy();
		}

		for (const auto& framebuffer : m_Framebuffers)
		{
			framebuffer->Destroy(device);
		}


		for (const auto& buffer : m_UniformBuffers)
		{
			buffer->Destroy();
		}

		for (const auto& buffer : m_StorageBuffers)
		{
			buffer->Destroy();
		}

		for (const auto& buffer : m_StagingBuffers)
		{
			buffer->Destroy();
		}


		for (const auto& pool : m_DescPools)
		{
			pool->Destroy();
		}

		for (const auto& pipelines : m_Pipelines)
		{
			pipelines->Destroy();

		}

		for (auto& shader : m_VulkanShaders)
		{
			shader->Destroy();
		}

		for (const auto& command : m_CommandLists)
		{
			command->Destroy(device);
		}


		


		result = device.waitIdle();
		vk::resultCheck(result, "Device failed to wait for idle");
		m_Allocator->Destroy();
		device.destroy();
		instance.destroySurfaceKHR(*m_Surface);

		instance.destroyDebugUtilsMessengerEXT(m_Messenger->GetMessenger(), nullptr, m_Instance->GetLoader());
		instance.destroy();
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
		
	}
}
