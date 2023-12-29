#include "boidpch.h"
#include "VkRenderAPI.h"

#include <spdlog/spdlog.h>

#include "Buffers.h"
#include "VkSwapChain.h"
#include "VulkanCommandBufferSet.h"
#include "VulkanFence.h"
#include "VulkanFramebuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanIndexBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanSemaphore.h"
#include "VulkanTexture2D.h"
#include "VulkanVertexBuffer.h"
#include "Engine/Renderer/RenderCommand.h"
#include "VulkanImage2D.h"
#include "VulkanKomputePipeline.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include "VulkanSamplerState.h"
#include "VulkanStagingBuffer.h"
#include "VulkanTexelBuffers.h"
#include "VulkanTexture3D.h"
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

	vk::Queue VkRenderAPI::GetVulkanQueue()
	{
		auto api = reinterpret_cast<VkRenderAPI*>(Get());
		return api->GetDevice()->GetGraphicsQueue();
	}

	vk::PhysicalDevice VkRenderAPI::GetVulkanPhysicalDevice()
	{
		auto api = reinterpret_cast<VkRenderAPI*>(Get());
		return api->GetPhysicalDevice()->GetPhysicalDevice();
	}

	vk::SurfaceKHR VkRenderAPI::GetVulkanSurface()
	{
		auto api = reinterpret_cast<VkRenderAPI*>(Get());
		return api->GetSurface()->GetSurface();
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




	VkRenderAPI::VkRenderAPI(const std::any& window): m_Window(window)
	{
		m_Instance = CreateRef<VkInstance>();
		m_Messenger = CreateRef<VkDebugMessenger>(m_Instance);
		m_Surface = CreateRef<VulkanSurfaceKHR>(m_Instance, window);
		m_PhysicalDevice = CreateRef<VulkanPhysicalDevice>(m_Instance, m_Surface);
		m_Device = CreateRef<VulkanDevice>(m_PhysicalDevice);
		m_Allocator = CreateRef<VulkanAllocatorInstance>(this);


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

		return framebuffer.As<Framebuffer>();
	}

	Ref<Texture2D> VkRenderAPI::CreateTexture2D(const TextureSettings& settings, const void* data)
	{
		auto texture = ALLOC_API(VulkanTexture2D, this, settings, data);
		//m_Textures2D.push_back(texture);

		return texture.As<Texture2D>();
	}

	Ref<Texture2D> VkRenderAPI::CreateTexture2D(const Ref<Image2D>& image)
	{
		auto texture = ALLOC_API(VulkanTexture2D, image);
		//m_Textures2D.push_back(texture);

		return texture.As<Texture2D>();
	}


	Ref<Texture3D> VkRenderAPI::CreateTexture3D(const void* data, const TextureSettings& settings)
	{
		auto texture = ALLOC_API(VulkanTexture3D, data, settings);

		return texture;
	}

	Ref<SamplerState> VkRenderAPI::CreateSampler(const SamplerSettings& settings)
	{
		auto sampler = ALLOC_API(VulkanSamplerState, this, settings);

		return sampler.As<SamplerState>();
	}


	Ref<Renderbuffer> VkRenderAPI::CreateRenderBuffer(const RenderbufferSettings& settings)
	{
		return nullptr;
	}

	Ref<UniformBuffer> VkRenderAPI::CreateUniformBuffer(uint32_t size, uint32_t binding)
	{
		auto buffer = ALLOC_API(VulkanUniformBuffer, this, size, binding);

		return buffer.As<UniformBuffer>();
	}

	Ref<StorageBuffer> VkRenderAPI::CreateStorageBuffer(uint32_t size)
	{
		auto buffer = ALLOC_API(VulkanShaderStorage,this, size);

		return buffer.As<StorageBuffer>();
	}

	Ref<IndexBuffer> VkRenderAPI::CreateIndexBuffer(const IndexDataType& type, uint32_t count,
	                                                const std::variant<uint32_t*, uint16_t*>& data)
	{

		auto indexBuffer = ALLOC_API(VulkanIndexBuffer, this, type, data, count);


		return indexBuffer.As<IndexBuffer>();
	}

	Ref<VertexBuffer> VkRenderAPI::CreateVertexBuffer(const void* data, uint32_t dataSize)
	{

		auto vertexBuffer = ALLOC_API(VulkanVertexBuffer, this, data, dataSize);


		return vertexBuffer.As<VertexBuffer>();
	}

	Ref<VertexBuffer> VkRenderAPI::CreateVertexBuffer(uint32_t dataSize)
	{

		auto vertexBuffer = ALLOC_API(VulkanVertexBuffer, this, dataSize);

		return vertexBuffer.As<VertexBuffer>();
	}

	Ref<VertexBufferArray> VkRenderAPI::CreateVertexBufferArray()
	{

		auto vaBuffer = ALLOC_API(VulkanVertexBufferArray);

		return vaBuffer.As<VertexBufferArray>();
	}

	Ref<CommandBufferSet> VkRenderAPI::CreateCommandList(const CommandListSettings& settings)
	{
		auto vulkanCommandList = ALLOC_API(VulkanCommandBufferSet, this, settings);

		return vulkanCommandList.As<CommandBufferSet>();
	}

	Ref<StagingBuffer> VkRenderAPI::CreateStagingBuffer(uint32_t size)
	{
		auto buffer = ALLOC_API(VulkanStagingBuffer, this, size);
		return buffer.As<StagingBuffer>();
	}

	Ref<TexelStorageBuffer> VkRenderAPI::CreateTexelStorageBuffer(const TexelBufferSettings& settings)
	{
		auto buffer = ALLOC_API(VulkanTexelStorageBuffer, settings);
		return buffer.As<TexelStorageBuffer>();
	}

	Ref<TexelUniformBuffer> VkRenderAPI::CreateTexelUniformBuffer(const TexelBufferSettings& settings)
	{
		auto buffer = ALLOC_API(VulkanTexelUniformBuffer, settings);
		return buffer.As<TexelUniformBuffer>();
	}

	Ref<GraphicsPipeline> VkRenderAPI::CreateGraphicsPipeline()
	{
		auto graphicsPipeline = ALLOC_API(VulkanGraphicsPipeline, this);


		return graphicsPipeline.As<GraphicsPipeline>();
	}

	Ref<KomputePipeline> VkRenderAPI::CreateKomputePipeline()
	{
		auto komputePipeline = ALLOC_API(VulkanKomputePipeline);


		return komputePipeline;
	}

	Ref<Swapchain> VkRenderAPI::CreateSwapChain(const SwapchainSettings& settings)
	{
		auto swapchain = ALLOC_API(VkSwapChain, this, settings);
		

		return swapchain.As<Swapchain>();
	}

	Ref<RenderPass> VkRenderAPI::CreateRenderPass(const RenderPassSettings& settings)
	{
		auto renderPass = ALLOC_API(VulkanRenderPass, this, settings);

		return renderPass.As<RenderPass>();
	}

	Ref<Fence> VkRenderAPI::CreateGraphicsFence()
	{
		auto fence = ALLOC_API(VulkanFence, this);

		return fence.As<Fence>();

	}

	Ref<Semaphore> VkRenderAPI::CreateGraphicsSemaphore()
	{

		auto semaphore = ALLOC_API(VulkanSemaphore, this);
	
		return semaphore.As<Semaphore>();
	}

	Ref<Image2D> VkRenderAPI::CreateImage2D(const ImageSettings& imageSettings)
	{
		auto image2D = ALLOC_API(VulkanImage2D, this, imageSettings);


		return image2D.As<Image2D>();
	}

	Ref<Shader> VkRenderAPI::CreateShader(const ShaderBinaryAndReflectionInfo& info)
	{
		auto shader = ALLOC_API(VulkanShader, this, info);
		return shader.As<Shader>();
	}

	Ref<PipelineDescriptorSetPool> VkRenderAPI::CreateDescriptorSetPool(const DescriptorSetPoolSettings& settings)
	{
		auto pool = ALLOC_API(VulkanPipelineDescriptorSetPool, this, settings);
		return pool.As<PipelineDescriptorSetPool>();
	}


	void VkRenderAPI::Destroy()
	{
		const auto instance = m_Instance->GetVKInstance();
		vk::Device device = (*m_Device);
		vk::Result result = device.waitIdle();
		vk::resultCheck(result, "Device failed to wait for idle");


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
