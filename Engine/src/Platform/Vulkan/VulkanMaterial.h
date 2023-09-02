#pragma once
#include "Engine/Engine/Base.h"
#include "Engine/Renderer/Material.h"
#include <vulkan/vulkan.hpp>


namespace Polyboid
{
	class VulkanGraphicsPipeline;
	class UniformBufferSet;
	class StagingBufferSet;
	class PipelineDescriptorSet;

	class VulkanMaterial : public Material
	{

	public:
		explicit VulkanMaterial( VulkanGraphicsPipeline* pipeline, const std::string& name);

		std::vector<Ref<PipelineDescriptorSet>> GetDescSets() override;
		void SetColor(const glm::vec3& color) override;
		void UploadMaterial() override;

		virtual void Recreate();

	private:

		void AllocateSets();

		VulkanGraphicsPipeline* m_Pipeline;
		const uint32_t MATERIAL_DATA_BIND_POINT = 0;

		std::vector<Ref<PipelineDescriptorSet>> m_DescSets;
		//Material Data
		Ref<UniformBufferSet> m_MaterialUniformBuffer;
		Ref<StagingBufferSet> m_MaterialStagingBuffer;
	};

}

