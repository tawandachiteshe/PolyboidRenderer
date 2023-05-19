#pragma once
#include <variant>

#include "Engine/Renderer/Buffer.h"
#include "Utils/VulkanAllocator.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanIndexBuffer : public IndexBuffer
	{

	private:
		vk::Buffer m_Handle;
		const VkRenderAPI* m_Context = nullptr;
		IndexDataType m_IndexType;
		uint32_t m_Count = 0;
		VmaAllocation m_Allocation{};
		uint32_t m_Size = 0;

	public:

		VulkanIndexBuffer(const VkRenderAPI* context, IndexDataType type,const std::variant<uint32_t*, uint16_t*>& data, uint32_t count);


		~VulkanIndexBuffer() override;
		void Bind() const override;
		void Unbind() const override;
		IndexDataType GetIndexDataType() override;
		uint32_t GetCount() const override;
		uint32_t GetSizeInBytes() const override;
		std::any GetHandle() const override;

		virtual void Destroy();
	};

}

