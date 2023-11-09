#include "boidpch.h"
#include "VulkanPipelineDescriptorSet.h"


#include <vulkan/vulkan_raii.hpp>

#include "Buffers.h"
#include "VkRenderAPI.h"
#include "VulkanImage2D.h"
#include "VulkanTexelBuffers.h"
#include "VulkanTexture2D.h"
#include "VulkanTexture3D.h"
#include "Engine/Renderer/RenderAPI.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	VulkanPipelineDescriptorSet::VulkanPipelineDescriptorSet(vk::DescriptorSet set, vk::PipelineLayout layout,
	                                                         const std::map<uint32_t, vk::WriteDescriptorSet>&
	                                                         writeSet): m_Set(set)
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
		const auto vulkanBuffer = buffer.As<VulkanUniformBuffer>();
		const auto bufferInfo = vulkanBuffer->GetVulkanDescBuffer();

		m_Buffers[binding] = (bufferInfo);
		writeDescriptor.pBufferInfo = &m_Buffers.at(binding);


		m_WriteSets.emplace_back(writeDescriptor);
	}

	void VulkanPipelineDescriptorSet::WriteStorageBuffer(uint32_t binding, const Ref<StorageBuffer>& buffer)
	{
		vk::WriteDescriptorSet writeDescriptor = m_WriteSetsMap.at(binding);
		writeDescriptor.dstSet = m_Set;

		const auto vulkanBuffer = buffer.As<VulkanShaderStorage>();

		const auto bufferInfo = vulkanBuffer->GetVulkanDescBuffer();
		m_Buffers[binding] = (bufferInfo);

		writeDescriptor.pBufferInfo = &m_Buffers.at(binding);

		m_WriteSets.emplace_back(writeDescriptor);
	}

	void VulkanPipelineDescriptorSet::WriteTexture2D(uint32_t binding, const Ref<Texture2D>& texture)
	{
		vk::WriteDescriptorSet writeDescriptor = m_WriteSetsMap.at(binding);
		writeDescriptor.dstSet = m_Set;

		const auto vulkanBuffer = texture.As<VulkanTexture2D>();
		const auto imageInfo = vulkanBuffer->GetVulkanDescriptorImageInfo();
		m_Images[binding] = (imageInfo);
		writeDescriptor.pImageInfo = &m_Images.at(binding);

		m_WriteSets.emplace_back(writeDescriptor);
	}

	void VulkanPipelineDescriptorSet::WriteTexture3D(uint32_t binding, const Ref<Texture3D>& texture)
	{
		vk::WriteDescriptorSet writeDescriptor = m_WriteSetsMap.at(binding);
		writeDescriptor.dstSet = m_Set;

		auto vulkanBuffer = texture.As<VulkanTexture3D>();
		auto imageInfo = vulkanBuffer->GetVulkanDescriptorImageInfo();
		m_Images[binding] = (imageInfo);
		writeDescriptor.pImageInfo = &m_Images.at(binding);

		m_WriteSets.emplace_back(writeDescriptor);
	}

	void VulkanPipelineDescriptorSet::WriteImage2D(uint32_t binding, const Ref<Image2D>& image2d)
	{
		vk::WriteDescriptorSet writeDescriptor = m_WriteSetsMap.at(binding);
		writeDescriptor.dstSet = m_Set;

		const auto vulkanBuffer = image2d.As<VulkanImage2D>();
		const auto imageInfo = vulkanBuffer->GetVulkanDescriptorImageInfo();
		m_Images[binding] = (imageInfo);
		writeDescriptor.pImageInfo = &m_Images.at(binding);

		m_WriteSets.emplace_back(writeDescriptor);
	}

	void VulkanPipelineDescriptorSet::WriteTexelStorageBuffer(uint32_t binding,
	                                                          const Ref<TexelStorageBuffer>& texelStorage)
	{
		vk::WriteDescriptorSet writeDescriptor = m_WriteSetsMap.at(binding);
		writeDescriptor.dstSet = m_Set;

		const auto vulkanBuffer = texelStorage.As<VulkanTexelStorageBuffer>();
		const auto bufferView = vulkanBuffer->GetBufferView();
		m_BufferViews[binding] = bufferView;

		writeDescriptor.pTexelBufferView = &m_BufferViews.at(binding);
		writeDescriptor.descriptorType = vk::DescriptorType::eStorageTexelBuffer;


		m_WriteSets.emplace_back(writeDescriptor);
	}

	void VulkanPipelineDescriptorSet::WriteTexelUniformBuffer(uint32_t binding,
	                                                          const Ref<TexelUniformBuffer>& texelStorage)
	{
		vk::WriteDescriptorSet writeDescriptor = m_WriteSetsMap.at(binding);
		writeDescriptor.dstSet = m_Set;

		const auto vulkanBuffer = texelStorage.As<VulkanTexelUniformBuffer>();
		const auto bufferView = vulkanBuffer->GetBufferView();
		m_BufferViews[binding] = bufferView;


		writeDescriptor.pTexelBufferView = &bufferView;
		writeDescriptor.descriptorType = vk::DescriptorType::eUniformTexelBuffer;

		m_WriteSets.emplace_back(writeDescriptor);
	}

	void VulkanPipelineDescriptorSet::Commit()
	{
		const auto api = reinterpret_cast<VkRenderAPI*>(RenderAPI::Get());
		const auto device = api->GetDevice()->GetVulkanDevice();

		device.updateDescriptorSets(m_WriteSets, {});
	}

	void VulkanPipelineDescriptorSet::Recreate()
	{
	}

	vk::PipelineLayout VulkanPipelineDescriptorSet::GetDescLayout()
	{
		return m_Layout;
	}
}
