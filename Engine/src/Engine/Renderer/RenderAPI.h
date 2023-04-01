#pragma once


#pragma once
#include <any>
#include <cstdint>
#include <variant>
#include <glm/vec3.hpp>

#include "Engine/Engine/Base.h"
#include "Swapchain.h"

namespace Polyboid
{
	class CommandList;
	struct RenderPassSettings;
	class RenderPass;
	struct ClearSettings;
	class Swapchain;
	class PipelineState;
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

		virtual Ref<Texture> CreateTexture2D(const TextureSettings& settings) = 0;
		virtual Ref<Texture> CreateTexture2D(const std::any& handle) = 0;
		virtual Ref<Texture3D> CreateTexture3D(const void** data, const TextureSettings& settings) = 0;
		virtual Ref<SamplerState> CreateSampler(const SamplerSettings& settings) = 0;
		virtual Ref<Framebuffer> CreateFrameBuffer(const FramebufferSettings& settings) = 0;
		virtual Ref<Framebuffer> CreateFrameBuffer(const FramebufferSettings& settings, const Ref<RenderPass>& renderPass) = 0;
		virtual Ref<Renderbuffer> CreateRenderBuffer(const RenderbufferSettings& settings) = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual Ref<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) = 0;
		virtual Ref<IndexBuffer> CreateIndexBuffer(const IndexDataType& type, uint32_t count, const std::variant<uint32_t*, uint16_t*>& data) = 0;
		virtual Ref<VertexBuffer> CreateVertexBuffer(const void* data, uint32_t dataSize) = 0;
		virtual Ref<VertexBuffer> CreateVertexBuffer(uint32_t dataSize) = 0;

		virtual Ref<VertexBufferArray> CreateVertexBufferArray() = 0;
		virtual Ref<PipelineState> CreatePipelineState() = 0;
		virtual Ref<Swapchain> CreateSwapChain(const SwapchainSettings& settings = SwapchainSettings{}) = 0;
		virtual Ref<RenderPass> CreateRenderPass(const RenderPassSettings& settings) = 0;
		virtual Ref<CommandList> CreateCommandList() = 0;

		//virtual void ClearRenderTarget(const ClearSettings& settings) = 0;

		virtual RenderAPIType GetRenderAPIType() = 0;
		virtual RenderAPIType GetRenderAPIType() const = 0;
		virtual ~RenderAPI() = default;

		static RenderAPI* Create(const RenderAPIType& renderType, const std::any& nativeWindow);
		static RenderAPI* Get();
	};


}
