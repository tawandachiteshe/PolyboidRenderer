#pragma once
#include "Engine/Renderer/VertexBufferArray.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VulkanIndexBuffer;
	class VulkanVertexBuffer;

	class VulkanVertexArray : public VertexBufferArray
	{
	private:
		std::vector<vk::VertexInputBindingDescription> m_BindingDescriptions;
		std::vector<vk::VertexInputAttributeDescription> m_VkLayouts;
		vk::PipelineVertexInputStateCreateInfo m_VertexInputInfo;
		std::vector<Ref<VulkanVertexBuffer>> m_VertexBuffers;
		Ref<VulkanIndexBuffer> m_IndexBuffer;

	public:
		bool IsUsingVertexPulling() override;
		void AddVertexBuffer(const Ref<VertexBuffer>& vbuffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& iBuffer) override;
		void SetIndexBuffer(const Ref<ShaderStorageBuffer>& buffer, uint32_t count, const IndexDataType& type) override;
		IndexDataType GetIndexDataType() override;
		uint32_t GetIndexCount() override;
		void SetIndexCount(uint32_t count) override;
		~VulkanVertexArray() override;
		void Bind() override;
		void UnBind() override;
	};

}

