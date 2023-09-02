#include "boidpch.h"
#include "VulkanMaterial.h"

#include "VkRenderAPI.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanPipelineDescriptorSet.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include "Engine/Renderer/BufferSet.h"
#include "Engine/Renderer/PipelineDescriptorSet.h"
#include "Engine/Renderer/RenderCommand.h"


namespace Polyboid
{
	VulkanMaterial::VulkanMaterial( VulkanGraphicsPipeline* pipeline, const std::string& name): m_Pipeline(pipeline)
	{

		m_Name = name;


		m_MaterialStagingBuffer = StagingBufferSet::Create(sizeof(m_Data));
		m_MaterialUniformBuffer = UniformBufferSet::Create(sizeof(m_Data));

		AllocateSets();


	

	}

	std::vector<Ref<PipelineDescriptorSet>> VulkanMaterial::GetDescSets()
	{
		return m_DescSets;
	}

	void VulkanMaterial::SetColor(const glm::vec3& color)
	{
		m_Data.color = color;
	}

	void VulkanMaterial::UploadMaterial()
	{
		m_MaterialStagingBuffer->SetData(&m_Data);
		RenderCommand::CopyStagingBuffer(m_MaterialStagingBuffer, m_MaterialUniformBuffer);
	}

	void VulkanMaterial::Recreate()
	{
		
		m_DescSets.clear();
		m_MaterialStagingBuffer->Recreate();
		m_MaterialUniformBuffer->Recreate();
		AllocateSets();

	

	}

	void VulkanMaterial::AllocateSets()
	{
		const uint32_t MATERIAL_SET = 1;

		vk::DescriptorSetLayout vulkanLayout = m_Pipeline->m_SetIndexWithLayout.at(MATERIAL_SET);
		vk::DescriptorSetAllocateInfo allocateInfo{};
		allocateInfo.descriptorPool = m_Pipeline->m_DescPool->GetVulkanHandle();
		allocateInfo.descriptorSetCount = m_Pipeline->m_DescPool->GetDescPoolSettings().ImageCount; //PER each frame

		vk::DescriptorSetLayout vulkanLayouts[] = { vulkanLayout, vulkanLayout, vulkanLayout };
		allocateInfo.pSetLayouts = vulkanLayouts;

		auto [allocResult, allocSets] = VkRenderAPI::GetVulkanDevice().allocateDescriptorSets(allocateInfo);
		
		vk::resultCheck(allocResult, "Failed to create Desc Sets");


		for (auto set : allocSets)
		{
			
			auto descSet = CreateRef<VulkanPipelineDescriptorSet>(set, m_Pipeline->m_PipelineLayout, m_Pipeline->m_DescWriteMap.at(MATERIAL_SET)).As<
				PipelineDescriptorSet>();
			m_DescSets.push_back(descSet);
		}

		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_DescSets.at(i)->WriteUniformBuffer(MATERIAL_DATA_BIND_POINT, m_MaterialUniformBuffer->Get(i));

		}


		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_DescSets.at(i)->Commit();
		}
	}
}
