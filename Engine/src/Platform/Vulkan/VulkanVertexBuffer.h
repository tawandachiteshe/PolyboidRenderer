#pragma once
#include "Engine/Renderer/Buffer.h"
#include <vulkan/vulkan_raii.hpp>

#include "VkRenderAPI.h"
#include <vma/vk_mem_alloc.h>

namespace Polyboid
{
	class VulkanVertexBuffer : public VertexBuffer
	{

	private:
		vk::Buffer m_Handle = nullptr;
		VmaAllocation m_Allocation = nullptr;
		const VkRenderAPI* m_Context = nullptr;
		BufferLayout m_Layout;
		uint32_t m_Size = 0;
		const void* m_Data = nullptr;

	public:

		void Init(const VkRenderAPI* context, const void* data, uint32_t size);
		void Recreate();

		VulkanVertexBuffer(const VkRenderAPI* context, const void* data, uint32_t size);
		VulkanVertexBuffer(const VkRenderAPI* context, uint32_t size);

		~VulkanVertexBuffer() override;
		void Bind() const override;
		void Unbind() const override;
		void SetData(const void* data, uint32_t size) override;
		const BufferLayout& GetLayout() const override;
		void SetLayout(const BufferLayout& layout) override;
		uint32_t GetSizeInBytes() const override;
		std::any GetHandle() const override;

		virtual void Destroy();
	};

}

