#pragma once
#include "Engine/Renderer/VertexBufferArray.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VulkanIndexBuffer;
	class VulkanVertexBuffer;

	class VulkanVertexBufferArray : public VertexBufferArray
	{
	private:
		std::vector<vk::VertexInputBindingDescription> m_BindingDescriptions;
		std::vector<vk::VertexInputAttributeDescription> m_VkLayouts;
		vk::PipelineVertexInputStateCreateInfo m_VertexInputInfo;
		std::vector<Ref<VulkanVertexBuffer>> m_VertexBuffers;
		std::vector<Ref<VertexBufferSet>> m_VertexBuffersSets;
		Ref<VulkanIndexBuffer> m_IndexBuffer;

	public:
		VulkanVertexBufferArray() = default;
		bool IsUsingVertexPulling() override;
		void AddVertexBuffer(const Ref<VertexBuffer>& vbuffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& iBuffer) override;
		void SetIndexBuffer(const Ref<StorageBuffer>& buffer, uint32_t count, const IndexDataType& type) override;
		IndexDataType GetIndexDataType() override;
		uint32_t GetIndexCount() override;
		void SetIndexCount(uint32_t count) override;
		void AddVertexBufferSet(const Ref<VertexBufferSet>& vbufferSet) override;
		~VulkanVertexBufferArray() override;
		Ref<IndexBuffer> GetIndexBuffer() override;
		void Recreate() override;
		void Destroy() override;
		RenderResourceType GetRenderResourceType() override;

		virtual vk::PipelineVertexInputStateCreateInfo GetVulkanInfo();

		void Bind() override;
		void UnBind() override;
	};

}

