#pragma once
#include "Engine/Renderer/KomputePipeline.h"
#include <vulkan/vulkan.hpp>

#include "VulkanDepthStencilState.h"
#include "VulkanShader.h"

namespace Polyboid
{
	class Image2D;
	class VulkanPipelineDescriptorSetPool;
	class PipelineDescriptorSet;
	class UniformBufferSet;
	class Texture2D;
	class StorageBufferSet;
	class VulkanShader;

	class VulkanKomputePipeline : public KomputePipeline
	{
	private:
		Ref<VulkanShader> m_ComputeShader;
		std::vector<vk::PipelineShaderStageCreateInfo> m_Stages;

		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_PipelineLayout;
		std::vector<vk::DescriptorSetLayout> m_DescriptorSetLayouts;
		std::unordered_map < uint32_t, std::vector<Ref<PipelineDescriptorSet>>> m_Sets;
		DescWriteMap m_DescWriteMap;
		vk::GraphicsPipelineCreateInfo m_CreatePipelineInfo{};

		std::map<uint32_t, std::vector<vk::DescriptorSetLayoutBinding>> m_Bindings;
		std::map<uint32_t, vk::DescriptorSetLayout> m_SetIndexWithLayout;
		std::vector<vk::PushConstantRange> m_PushConstantRanges;
		Ref<VulkanPipelineDescriptorSetPool> m_DescPool;
		std::vector<Ref<PipelineDescriptorSet>> m_PipeDescSets;

		std::unordered_map<uint32_t, std::unordered_map<uint32_t, Ref<UniformBufferSet>>> m_UniformBufferSets;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, Ref<StorageBufferSet>>> m_StorageBufferSets;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, Ref<Texture2D>>> m_TextureSets;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, Ref<Image2D>>> m_Image;

		std::unordered_map<uint32_t, std::unordered_map<uint32_t, Ref<TexelUniformBuffer>>> m_UniformTexel;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, Ref<TexelStorageBuffer>>> m_StorageTexel;

	public:
		VulkanKomputePipeline();

		void Init();
		void SetComputeShader(const Ref<Shader>& computeShader) override;
		void Destroy() override;
		void Recreate() override;
		vk::PipelineLayout GetPipelineLayout();
		std::any GetHandle() override;

		std::vector<Ref<PipelineDescriptorSet>> AllocateDescriptorSets(uint32_t setBinding) override;
		std::vector<Ref<PipelineDescriptorSet>> GetDescriptorSets(uint32_t set) override;

		void BindUniformBufferSet(uint32_t binding, const Ref<UniformBufferSet>& bufferSet,
			uint32_t setBinding) override;
		void BindStorageBufferSet(uint32_t binding, const Ref<StorageBufferSet>& bufferSet,
			uint32_t setBinding) override;
		void BindTexture2D(uint32_t binding, const Ref<Texture2D>& bufferSet, uint32_t setBinding) override;

		void BindImage2D(uint32_t binding, const Ref<Image2D>& bufferSet, uint32_t setBinding) override;

		void BindTexelUniformBuffer(uint32_t binding, const Ref<TexelUniformBuffer>& bufferSet,
			uint32_t setBinding) override;
		void BindTexelStorageBuffer(uint32_t binding, const Ref<TexelStorageBuffer>& bufferSet,
			uint32_t setBinding) override;

		void WriteSetResourceBindings(uint32_t set) override;


		void Bake() override;
		~VulkanKomputePipeline() override;
	};

}

