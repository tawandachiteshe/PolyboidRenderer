#pragma once
#include "Engine/Renderer/PipelineDescriptorSetPool.h"
#include <vulkan/vulkan_raii.hpp>

namespace Polyboid
{
	class VulkanPipelineDescriptorSet;
	class VkRenderAPI;

	class VulkanPipelineDescriptorSetPool : public PipelineDescriptorSetPool
	{
	private:
		const VkRenderAPI* m_Context = nullptr;
		DescriptorSetPoolSettings m_Settings;
		vk::DescriptorPool m_DescPool;
		std::vector<Ref<PipelineDescriptorSet>> m_Sets;

	public:

		VulkanPipelineDescriptorSetPool(const VkRenderAPI* context, DescriptorSetPoolSettings settings);

		Ref<PipelineDescriptorSet> GetDescriptorSet(uint32_t index) override;
		std::any GetHandle() override;
		void Reset() override;
		virtual void Destroy();
		~VulkanPipelineDescriptorSetPool() override = default;
		DescriptorSetPoolSettings GetDescPoolSettings() override;
		virtual vk::DescriptorPool GetVulkanHandle();
	};

}

