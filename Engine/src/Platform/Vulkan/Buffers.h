#pragma once
#include <vulkan/vulkan.hpp>
#include "Engine/Renderer/UniformBuffer.h"
#include <vma/vk_mem_alloc.h>

#include "Engine/Renderer/Buffer.h"

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanUniformBuffer : public UniformBuffer
	{
	private:
		vk::Buffer m_UniformBuffer = nullptr;
		vk::DescriptorBufferInfo m_DescBufferInfo;
		const VkRenderAPI* m_Context = nullptr;
		VmaAllocation m_Allocation = nullptr;
		vk::DeviceMemory m_Memory;
		uint32_t m_Size = 0, m_Slot = 0;

	public:
		VulkanUniformBuffer(const VkRenderAPI* context, uint32_t size, uint32_t slot = 0);

		void Init(const VkRenderAPI* context, uint32_t size);
		void Recreate() override;
		void Destroy() override;

		void SetData(const void* data, uint32_t size, uint32_t offset) override;
		uint32_t GetBindingSlot() override;
		uint32_t GetDataSize() override;
	
		std::any GetHandle() override;
		virtual  vk::DescriptorBufferInfo GetVulkanDescBuffer();

		RenderResourceType GetRenderResourceType() override;
	};


	class VulkanShaderStorage : public StorageBuffer
	{
		VmaAllocation m_Allocation = nullptr;
		VmaAllocator m_Allocator = nullptr;
		uint32_t m_Size = 0, m_Slot = 0;
		vk::Buffer m_StorageBuffer = nullptr;
		vk::DescriptorBufferInfo m_DescBufferInfo;
		const VkRenderAPI* m_Context = nullptr;

	public:
		VulkanShaderStorage(const VkRenderAPI* context, uint32_t size);
		void Init(const VkRenderAPI* context, uint32_t size);
		void Recreate() override;
		void Destroy() override;
		void Bind(uint32_t slot) const override;
		void Unbind() const override;
		void SetData(const void* data, uint32_t sizeData, uint32_t offset) override;
		std::any GetHandle() override;
		virtual vk::DescriptorBufferInfo GetVulkanDescBuffer();
		~VulkanShaderStorage() override;
		RenderResourceType GetRenderResourceType() override;
	};


	
}
