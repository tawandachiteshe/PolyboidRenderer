#include "boidpch.h"
#include "VulkanGraphicsPipeline.h"

#include <spdlog/spdlog.h>


namespace Polyboid
{
	VulkanGraphicsPipeline::VulkanGraphicsPipeline(const VkRenderAPI* context)
	{



		vk::PipelineLayoutCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.pPushConstantRanges = nullptr;
		pipelineCreateInfo.pushConstantRangeCount = 0;
		pipelineCreateInfo.pSetLayouts = nullptr;
		pipelineCreateInfo.setLayoutCount = 0;

		

		vk::GraphicsPipelineCreateInfo createInfo;
		


	}

	void VulkanGraphicsPipeline::SetShader(const ShaderType& type, const Ref<Shader>& shader)
	{
		m_Shaders[type] = shader;
	}

	void VulkanGraphicsPipeline::SetShader(const Ref<Shader>& shader)
	{
		m_Shaders[shader->GetType()] = shader;
	}

	std::shared_ptr<Shader> VulkanGraphicsPipeline::GetShader(const ShaderType& type) const
	{
		if (!m_Shaders.contains(type))
		{
			__debugbreak();
			spdlog::info("Shader not found");
		}

		return m_Shaders.at(type);
	}

	const PipelineState::ShaderMap& VulkanGraphicsPipeline::GetShaders() const
	{
		return m_Shaders;
	}

	void VulkanGraphicsPipeline::SetBlendState(const BlendState& blendState)
	{
	}

	BlendState& VulkanGraphicsPipeline::GetBlendState()
	{
		return m_BlendState;
	}

	void VulkanGraphicsPipeline::SetRasterizerState(const RasterizerState& rasterizerState)
	{
		m_RasterizerState = dynamic_cast<const VulkanRasterizerState&>(rasterizerState);
	}

	RasterizerState& VulkanGraphicsPipeline::GetRasterizerState()
	{
		return m_RasterizerState;
	}

	void VulkanGraphicsPipeline::SetDepthStencilState(const DepthStencilState& depthStencilState)
	{
		m_DepthStencil = dynamic_cast<const VulkanDepthStencilState&>(depthStencilState);
	}

	DepthStencilState& VulkanGraphicsPipeline::GetDepthStencilState()
	{
		return m_DepthStencil;
	}

	void VulkanGraphicsPipeline::SetVertexArray(const Ref<VertexBufferArray>& va)
	{
		m_VertexInput = va;
	}

	
	Ref<VertexBufferArray>& VulkanGraphicsPipeline::GetVertexArray()
	{
		return  m_VertexInput;
		//dynamic_cast<std::nullptr_t&>(nullptr);
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
	}

	void VulkanGraphicsPipeline::Bind()
	{
	}

	void VulkanGraphicsPipeline::UnBind()
	{
	}
}
