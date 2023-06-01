#include "boidpch.h"
#include "VulkanPipelineDescriptorSet.h"


#include <vulkan/vulkan_raii.hpp>

#include "Buffers.h"
#include "VkRenderAPI.h"
#include "VulkanTexture2D.h"
#include "Engine/Renderer/RenderAPI.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{

	VulkanPipelineDescriptorSet::VulkanPipelineDescriptorSet(vk::DescriptorSet set, vk::PipelineLayout layout, const std::map<uint32_t, vk::WriteDescriptorSet>& writeSet): m_Set(set)
	, m_WriteSetsMap(writeSet), m_Layout(layout)
	{
	}

	std::any VulkanPipelineDescriptorSet::GetHandle()
	{
		return m_Set;
	}

	void VulkanPipelineDescriptorSet::WriteUniformBuffer(uint32_t binding, const Ref<UniformBuffer>& buffer)
	{
		vk::WriteDescriptorSet writeDescriptor = m_WriteSetsMap.at(binding);
		writeDescriptor.dstSet = m_Set;
		auto vulkanBuffer = buffer.As<VulkanUniformBuffer>();
		auto bufferInfo = vulkanBuffer->GetVulkanDescBuffer();

		m_Buffers[binding] = (bufferInfo);
		writeDescriptor.pBufferInfo = &m_Buffers.at(binding);


		m_WriteSets.emplace_back(writeDescriptor);

	}

	void VulkanPipelineDescriptorSet::WriteStorageBuffer(uint32_t binding, const Ref<StorageBuffer>& buffer)
	{
		vk::WriteDescriptorSet writeDescriptor = m_WriteSetsMap.at(binding);
		writeDescriptor.dstSet = m_Set;

		auto vulkanBuffer = buffer.As<VulkanShaderStorage>();
		
		auto bufferInfo = vulkanBuffer->GetVulkanDescBuffer();
		m_Buffers[binding] = (bufferInfo);

		writeDescriptor.pBufferInfo = &m_Buffers.at(binding);

		m_WriteSets.emplace_back(writeDescriptor);
	}

	void VulkanPipelineDescriptorSet::WriteTexture2D(uint32_t binding, const Ref<Texture>& texture)
	{
		vk::WriteDescriptorSet writeDescriptor = m_WriteSetsMap.at(binding);
		writeDescriptor.dstSet = m_Set;

		auto vulkanBuffer = texture.As<VulkanTexture2D>();
		auto imageInfo = vulkanBuffer->GetVulkanDescriptorImageInfo();
		m_Images[binding] = (imageInfo);
		writeDescriptor.pImageInfo = &m_Images.at(binding);

		m_WriteSets.emplace_back(writeDescriptor);
	}

	void VulkanPipelineDescriptorSet::Commit()
	{
		auto api = reinterpret_cast<VkRenderAPI*>(RenderAPI::Get());
		auto device = api->GetDevice()->GetVulkanDevice();

		device.updateDescriptorSets(m_WriteSets, {});

	}

	vk::PipelineLayout VulkanPipelineDescriptorSet::GetDescLayout()
	{
		return m_Layout;
	}
}
