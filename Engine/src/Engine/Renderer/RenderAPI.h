#pragma once


#pragma once
#include <any>
#include <cstdint>
#include <variant>

#include "Shader.h"
#include "Engine/Engine/Base.h"
#include "Swapchain.h"

namespace Polyboid
{
	class StagingBuffer;
	struct DescriptorSetPoolSettings;
	class PipelineDescriptorSetPool;
	struct CommandListSettings;
	class CommandBuffer;
	class Fence;
	class CommandBufferSet;
	struct RenderPassSettings;
	class RenderPass;
	struct ClearSettings;
	class Swapchain;
	class GraphicsPipeline;
	class VertexBufferArray;
	class VertexBuffer;
	class IndexBuffer;
	class Renderbuffer;
	struct FramebufferSettings;
	struct RenderbufferSettings;
	class UniformBuffer;
	class Framebuffer;
	struct SamplerSettings;
	class SamplerState;
	class Texture3D;
	struct TextureSettings;
	class Texture;

	enum class PrimitiveType
	{
		Points = 0,
		Lines,
		LineLoop,
		LineStrip,
		Triangles,
		TriangleStrip,
		TriangleFan,
		Patches
	};

	enum class IndexDataType
	{
		UnsignedShort = 0,
		UnsignedInt
	};

	enum class RenderAPIType
	{
		Opengl,
		Vulkan,
		Metal,
		Dx11,
		Dx12,
		
	};

	class RenderAPI
	{
	private:
		static RenderAPI* s_RenderAPI;

	public:

		virtual Ref<Texture> CreateTexture2D(const TextureSettings& settings, const void* data = nullptr) = 0;
		virtual Ref<Texture> CreateTexture2D(const std::any& handle) = 0;
		virtual Ref<Texture3D> CreateTexture3D(const void** data, const TextureSettings& settings) = 0;
		virtual Ref<SamplerState> CreateSampler(const SamplerSettings& settings) = 0;
		virtual Ref<Framebuffer> CreateFrameBuffer(const FramebufferSettings& settings) = 0;
		virtual Ref<Framebuffer> CreateFrameBuffer(const Ref<RenderPass>& renderPass) = 0;
		virtual Ref<Renderbuffer> CreateRenderBuffer(const RenderbufferSettings& settings) = 0;

		virtual Ref<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) = 0;
		virtual Ref<IndexBuffer> CreateIndexBuffer(const IndexDataType& type, uint32_t count, const std::variant<uint32_t*, uint16_t*>& data) = 0;
		virtual Ref<VertexBuffer> CreateVertexBuffer(const void* data, uint32_t dataSize) = 0;
		virtual Ref<VertexBuffer> CreateVertexBuffer(uint32_t dataSize) = 0;

		virtual Ref<VertexBufferArray> CreateVertexBufferArray() = 0;
		virtual Ref<GraphicsPipeline> CreatePipelineState() = 0;
		virtual Ref<Swapchain> CreateSwapChain(const SwapchainSettings& settings = SwapchainSettings{}) = 0;
		virtual Ref<RenderPass> CreateRenderPass(const RenderPassSettings& settings) = 0;
		virtual Ref<CommandBufferSet> CreateCommandList(const CommandListSettings& settings) = 0;
		virtual Ref<PipelineDescriptorSetPool> CreateDescriptorSetPool(const DescriptorSetPoolSettings& settings) = 0;

		virtual Ref<Fence> CreateGraphicsFence() = 0;
		virtual Ref<Semaphore> CreateGraphicsSemaphore() = 0;
		virtual Ref<Image2D> CreateImage2D(const ImageSettings& imageSettings) = 0;
		virtual Ref<Shader> CreateShader(const ShaderBinaryAndReflectionInfo& info) = 0;
		virtual Ref<StagingBuffer> CreateStagingBuffer(uint32_t size) = 0;

		virtual Ref<StorageBuffer> CreateStorageBuffer(uint32_t size) = 0;

		virtual RenderAPIType GetRenderAPIType() = 0;
		virtual RenderAPIType GetRenderAPIType() const = 0;
		virtual ~RenderAPI() = default;

		static RenderAPI* Create(const RenderAPIType& renderType, const std::any& nativeWindow);
		static RenderAPI* Get();

		virtual void Destroy() = 0;
	};


}
