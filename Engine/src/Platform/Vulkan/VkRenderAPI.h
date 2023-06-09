#pragma once
#include "VulkanShader.h"
#include "Engine/Renderer/RenderAPI.h"
#include <vma/vk_mem_alloc.h>

namespace Polyboid
{
	class VulkanKomputePipeline;
	class VulkanStagingBuffer;
	class VulkanPipelineDescriptorSetPool;
	class VulkanGraphicsPipeline;
	class VulkanImage2D;
	class VulkanSemaphore;
	class VulkanFence;
	class Fence;
	class VulkanDescriptorPool;
	class VulkanVertexBuffer;
	class VulkanIndexBuffer;
	class VulkanTexture2D;
	class VulkanCommandBufferSet;
	class VulkanFramebuffer;
	class VulkanRenderPass;
	class VulkanAllocatorInstance;
	class VkSwapChain;
	class VulkanSurfaceKHR;
	class VulkanDevice;
	class VkDebugMessenger;
	class VkInstance;
	class VulkanPhysicalDevice;

	class VkRenderAPI : public RenderAPI
	{
	private:
		std::vector<Ref<VkSwapChain>> m_Swapchains;
		std::vector<Ref<VulkanRenderPass>> m_RenderPasses;
		std::vector<Ref<VulkanFramebuffer>> m_Framebuffers;
		std::vector<Ref<VulkanCommandBufferSet>> m_CommandLists;
		std::vector<Ref<VulkanTexture2D>> m_Textures2D;
		std::vector<Ref<VulkanIndexBuffer>> m_IndexBuffers;
		std::vector<Ref<VulkanVertexBuffer>> m_VertexBuffers;
		std::vector<Ref<VulkanFence>> m_Fences;
		std::vector<Ref<VulkanSemaphore>> m_Semaphores;
		std::vector<Ref<VulkanImage2D>> m_Image2Ds;
		std::vector<Ref<VulkanShader>> m_VulkanShaders;
		std::vector<Ref<VulkanGraphicsPipeline>> m_GraphicsPipelines;
		std::vector<Ref<VulkanKomputePipeline>> m_KomputePipelines;
		std::vector<Ref<VulkanPipelineDescriptorSetPool>> m_DescPools;
		std::vector<Ref<VulkanShaderStorage>> m_StorageBuffers;
		std::vector<Ref<VulkanUniformBuffer>> m_UniformBuffers;
		std::vector<Ref<VulkanStagingBuffer>> m_StagingBuffers;
		std::vector<vk::CommandBuffer> m_CommandBuffersBatching;


	private:
		Ref<VkInstance> m_Instance;
		Ref<VkDebugMessenger> m_Messenger;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurfaceKHR> m_Surface;
		Ref<VulkanAllocatorInstance> m_Allocator;
		std::any m_Window;
		bool m_CanContinueRender = true;
		uint32_t m_SwapchainImageIndex = 0;


	public:

		//Vulkan specific
		[[nodiscard]] Ref<VkInstance> GetInstance() const { return  m_Instance; }
		[[nodiscard]] Ref<VulkanPhysicalDevice> GetPhysicalDevice() const { return m_PhysicalDevice; }
		[[nodiscard]] Ref<VulkanAllocatorInstance> GetAllocator() const { return m_Allocator; }
		[[nodiscard]] Ref<VulkanDevice> GetDevice() const;
		[[nodiscard]] std::any GetWindow() const { return m_Window; }
		[[nodiscard]] Ref<VulkanSurfaceKHR> GetSurface() const;

		static vk::Device GetVulkanDevice();
		static vk::Queue GetVulkanQueue();
		static vk::PhysicalDevice GetVulkanPhysicalDevice();
		static vk::SurfaceKHR GetVulkanSurface();
		static vk::Instance GetVulkanInstance();
		static VmaAllocator GetVulkanAllocator();


		VkRenderAPI(const std::any& window);

		Ref<Framebuffer>
		CreateFrameBuffer(const FramebufferSettings& settings) override;
		Ref<Framebuffer>
		CreateFrameBuffer(const Ref<RenderPass>& renderPass) override;
		Ref<Texture> CreateTexture2D(const TextureSettings& settings, const void* data = nullptr) override;
		Ref<Texture> CreateTexture2D(const std::any& handle) override;
		Ref<Texture3D> CreateTexture3D(const void** data, const TextureSettings& settings) override;
		Ref<SamplerState> CreateSampler(const SamplerSettings& settings) override;


		Ref<Renderbuffer> CreateRenderBuffer(const RenderbufferSettings& settings) override;
		Ref<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) override;
		Ref<StorageBuffer> CreateStorageBuffer(uint32_t size) override;
		Ref<IndexBuffer> CreateIndexBuffer(const IndexDataType& type, uint32_t count,
			const std::variant<uint32_t*, uint16_t*>& data) override;

		Ref<VertexBuffer> CreateVertexBuffer(const void* data, uint32_t dataSize) override;
		Ref<VertexBuffer> CreateVertexBuffer(uint32_t dataSize) override;
		Ref<VertexBufferArray> CreateVertexBufferArray() override;
		Ref<CommandBufferSet> CreateCommandList(const CommandListSettings& settings) override;
		Ref<StagingBuffer> CreateStagingBuffer(uint32_t size) override;

		Ref<GraphicsPipeline> CreateGraphicsPipeline() override;
		Ref<KomputePipeline> CreateKomputePipeline() override;
		Ref<Swapchain> CreateSwapChain(const SwapchainSettings& settings) override;
		Ref<RenderPass> CreateRenderPass(const RenderPassSettings& settings) override;

		Ref<Fence> CreateGraphicsFence() override;
		Ref<Semaphore> CreateGraphicsSemaphore() override;
		Ref<Image2D> CreateImage2D(const ImageSettings& imageSettings) override;
		Ref<Shader> CreateShader(const ShaderBinaryAndReflectionInfo& info) override;
		Ref<PipelineDescriptorSetPool> CreateDescriptorSetPool(const DescriptorSetPoolSettings& settings) override;;

		void Destroy() override;

		RenderAPIType GetRenderAPIType() override;
		RenderAPIType GetRenderAPIType() const override;
		~VkRenderAPI() override;



		friend class VkSwapChain;
		friend class VulkanCommandBuffer;
		friend class VulkanRenderPass;
	};
}
