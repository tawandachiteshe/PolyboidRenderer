﻿#include "boidpch.h"
#include "VulkanVertexBufferArray.h"

#include "VulkanIndexBuffer.h"


namespace Polyboid
{

	vk::Format GetShaderDataType(const ShaderDataType& type)
	{
		switch (type)
		{
		case ShaderDataType::None: return vk::Format::eUndefined;
		case ShaderDataType::Float: return vk::Format::eR32Sfloat;
		case ShaderDataType::Float2:  return vk::Format::eR32G32Sfloat;
		case ShaderDataType::Float3: return vk::Format::eR32G32B32Sfloat;
		case ShaderDataType::Float4: return vk::Format::eR32G32B32A32Sfloat;
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
			//TODO: deprecate this i will use ssbos instead
			return  vk::Format::eUndefined;
		case ShaderDataType::Int:
			return vk::Format::eR32Sint;
		case ShaderDataType::Int2:
			return vk::Format::eR32G32Sint;
		case ShaderDataType::Int3:
			return vk::Format::eR32G32B32Sint;
		case ShaderDataType::Int4:
			return vk::Format::eR32G32B32A32Sint;
			
		case ShaderDataType::Bool:
			return vk::Format::eR32Sint;
		}
	}

	bool VulkanVertexBufferArray::IsUsingVertexPulling()
	{
		return false;
	}

	void VulkanVertexBufferArray::AddVertexBuffer(const Ref<VertexBuffer>& vbuffer)
	{

		uint32_t size = m_VertexBuffers.size();
		auto& layout = vbuffer->GetLayout();

		vk::VertexInputBindingDescription description;
		description.binding = 0;
		description.inputRate = vk::VertexInputRate::eVertex;
		description.stride = layout.GetStride();
		

		uint32_t count = 0;

		for (auto element : layout)
		{
			vk::VertexInputAttributeDescription vkLayout;
			vkLayout.binding = 0;
			vkLayout.format = GetShaderDataType(element.Type);
			vkLayout.offset = static_cast<uint32_t>(element.Offset);
			vkLayout.location = count;
			m_VkLayouts.push_back(vkLayout);
			
			count++;
		}

		m_BindingDescriptions.push_back(description);

		m_VertexInputInfo.vertexBindingDescriptionCount = 1;
		m_VertexInputInfo.pVertexBindingDescriptions = m_BindingDescriptions.data();

		m_VertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_VkLayouts.size());
		m_VertexInputInfo.pVertexAttributeDescriptions = m_VkLayouts.data();

		auto vulkanBuffer = std::reinterpret_pointer_cast<VulkanVertexBuffer>(vbuffer);
		
		m_VertexBuffers.push_back(vulkanBuffer);

	}

	void VulkanVertexBufferArray::SetIndexBuffer(const Ref<IndexBuffer>& iBuffer)
	{
		m_IndexBuffer = std::reinterpret_pointer_cast<VulkanIndexBuffer>(iBuffer);
	}

	void VulkanVertexBufferArray::SetIndexBuffer(const Ref<StorageBuffer>& buffer, uint32_t count,
		const IndexDataType& type)
	{

	}

	IndexDataType VulkanVertexBufferArray::GetIndexDataType()
	{
		return IndexDataType::UnsignedInt;
	}

	uint32_t VulkanVertexBufferArray::GetIndexCount()
	{
		return m_IndexBuffer->GetCount();
	}

	void VulkanVertexBufferArray::SetIndexCount(uint32_t count)
	{
		
	}

	VulkanVertexBufferArray::~VulkanVertexBufferArray()
	{
	}

	vk::PipelineVertexInputStateCreateInfo VulkanVertexBufferArray::GetVulkanInfo()
	{
		return m_VertexInputInfo;
	}

	void VulkanVertexBufferArray::Bind()
	{
	}

	void VulkanVertexBufferArray::UnBind()
	{
	}
}