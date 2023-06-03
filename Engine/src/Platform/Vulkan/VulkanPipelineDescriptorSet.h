#pragma once
#include "Engine/Renderer/PipelineDescriptorSet.h"
#include <vulkan/vulkan_raii.hpp>

namespace Polyboid
{
	class VulkanPipelineDescriptorSet : public PipelineDescriptorSet
	{

	private:
		vk::DescriptorSet m_Set;
		std::vector<vk::WriteDescriptorSet> m_WriteSets;
		std::map<uint32_t, vk::WriteDescriptorSet> m_WriteSetsMap;
		std::map<uint32_t,vk::DescriptorBufferInfo> m_Buffers;
		std::map<uint32_t,vk::DescriptorImageInfo> m_Images;
		
		vk::PipelineLayout m_Layout;

	public:
		explicit VulkanPipelineDescriptorSet(vk::DescriptorSet set, vk::PipelineLayout layout ,const std::map<uint32_t, vk::WriteDescriptorSet>& writeSet);
		std::any GetHandle() override;
		void WriteUniformBuffer(uint32_t binding, const Ref<UniformBuffer>& buffer) override;
		void WriteStorageBuffer(uint32_t binding, const Ref<StorageBuffer>& buffer) override;
		void WriteTexture2D(uint32_t binding, const Ref<Texture>& texture) override;
		void Commit() override;
		void Recreate();
		~VulkanPipelineDescriptorSet() override = default;

		virtual  vk::PipelineLayout GetDescLayout();
	};

}

