#pragma once
#include "VulkanShader.h"
#include "Engine/Renderer/RenderAPI.h"
#include "Utils/VulkanSyncObjects.h"


namespace Polyboid
{
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
	class VulkanCommandList;
	class VulkanFramebuffer;
	class VulkanRenderPass;
	class VulkanAllocator;
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
		std::vector<Ref<VulkanCommandList>> m_CommandLists;
		std::vector<Ref<VulkanTexture2D>> m_Textures2D;
		std::vector<Ref<VulkanIndexBuffer>> m_IndexBuffers;
		std::vector<Ref<VulkanVertexBuffer>> m_VertexBuffers;
		std::vector<Ref<VulkanFence>> m_Fences;
		std::vector<Ref<VulkanSemaphore>> m_Semaphores;
		std::vector<Ref<VulkanImage2D>> m_Image2Ds;
		std::vector<Ref<VulkanShader>> m_VulkanShaders;
		std::vector<Ref<VulkanGraphicsPipeline>> m_Pipelines;
		std::vector<Ref<VulkanPipelineDescriptorSetPool>> m_DescPools;
		std::vector<Ref<VulkanShaderStorage>> m_StorageBuffers;
		std::vector<Ref<VulkanUniformBuffer>> m_UniformBuffers;


	private:
		Ref<VkInstance> m_Instance;
		Ref<VkDebugMessenger> m_Messenger;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurfaceKHR> m_Surface;
		Ref<VulkanAllocator> m_Allocator;
		std::any m_Window;


	public:

		//Vulkan specific
		[[nodiscard]] Ref<VkInstance> GetInstance() const { return  m_Instance; }
		[[nodiscard]] Ref<VulkanPhysicalDevice> GetPhysicalDevice() const { return m_PhysicalDevice; }
		[[nodiscard]] Ref<VulkanAllocator> GetAllocator() const { return m_Allocator; }
		[[nodiscard]] Ref<VulkanDevice> GetDevice() const;
		[[nodiscard]] std::any GetWindow() const { return m_Window; }
		[[nodiscard]] Ref<VulkanSurfaceKHR> GetSurface() const;

		void SubmitCommandBuffer(const std::vector<Ref<CommandList>>& cmdList, const Ref<Semaphore>& imageAvailable, const Ref<Semaphore>& renderFinished,
			const Ref<Fence>& inFlight) override;

		void SubmitCommandBuffer(const std::vector<Ref<CommandList>>& cmdList) override;


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
		Ref<CommandList> CreateCommandList(const CommandListSettings& settings) override;

		Ref<PipelineState> CreatePipelineState() override;
		Ref<Swapchain> CreateSwapChain(const SwapchainSettings& settings) override;
		Ref<RenderPass> CreateRenderPass(const RenderPassSettings& settings) override;

		Ref<Fence> CreateGraphicsFence() override;
		Ref<Semaphore> CreateGraphicsSemaphore() override;
		Ref<Image2D> CreateImage2D(const ImageSettings& imageSettings) override;
		Ref<Shader> CreateShader(const ShaderBinaryAndReflectionInfo& info) override;
		Ref<PipelineDescriptorSetPool> CreateDescriptorSetPool(const DescriptorSetPoolSettings& settings) override;
		

		RenderAPIType GetRenderAPIType() override;
		RenderAPIType GetRenderAPIType() const override;
		~VkRenderAPI() override;



		friend class VkSwapChain;
		friend class VulkanCommandBuffer;
		friend class VulkanRenderPass;
	};
}
