#pragma once


#pragma once
#include <any>
#include <cstdint>
#include <variant>
#include <glm/vec3.hpp>

#include "Engine/Engine/Base.h"


namespace Polyboid
{
	struct RenderTargetSettings;
	class RenderTarget;
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
	public:

		virtual Ref<Texture> CreateTexture2D(const TextureSettings& settings) = 0;
		virtual Ref<Texture3D> CreateTexture3D(const void** data, const TextureSettings& settings) = 0;
		virtual Ref<SamplerState> CreateSampler(const SamplerSettings& settings) = 0;
		virtual Ref<Framebuffer> CreateFrameBuffer(const FramebufferSettings& settings) = 0;
		virtual Ref<Renderbuffer> CreateRenderBuffer(const RenderbufferSettings& settings) = 0;


		virtual Ref<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) = 0;
		virtual Ref<IndexBuffer> CreateIndexBuffer(const IndexDataType& type, uint32_t count, const std::variant<uint32_t*, uint16_t*>& data) = 0;
		virtual Ref<VertexBuffer> CreateVertexBuffer(const void* data, uint32_t dataSize) = 0;
		virtual Ref<VertexBuffer> CreateVertexBuffer(uint32_t dataSize) = 0;

		virtual Ref<VertexBufferArray> CreateVertexBufferArray() = 0;
		virtual Ref<PipelineState> CreatePipelineState() = 0;
		virtual Ref<Swapchain> CreateSwapChain(const std::any& window) = 0;
		virtual Ref<RenderTarget> CreateRenderTarget(const RenderTargetSettings& settings) = 0;

		virtual void BeginRenderPass(const Ref<RenderTarget>& renderTarget) = 0;
		virtual void EndRenderPass(const Ref<RenderTarget>& renderTarget) = 0;

		

		virtual void DrawIndexed(const PrimitiveType& primitiveType, const IndexDataType& indexDataType, uint32_t count) = 0;
		virtual void DrawArrays(const PrimitiveType& primitiveType, uint32_t vertexCount) = 0;
		virtual void Dispatch(const glm::uvec3& groups) = 0;

		//virtual void ClearRenderTarget(const ClearSettings& settings) = 0;

		virtual RenderAPIType GetRenderAPIType() = 0;
		virtual ~RenderAPI() = default;

		static Ref<RenderAPI> Create(const RenderAPIType& renderType, const std::any& nativeWindow);
	};


}
