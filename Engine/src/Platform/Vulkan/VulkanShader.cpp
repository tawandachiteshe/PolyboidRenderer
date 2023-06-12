#include "boidpch.h"
#include "VulkanShader.h"
#include <vulkan/vulkan_raii.hpp>

#include "VkRenderAPI.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	VulkanShader::VulkanShader(const VkRenderAPI* context, ShaderBinaryAndReflectionInfo info): m_Context(context), m_ShaderInfo(
		std::move(info))
	{

		auto device = context->GetDevice()->GetVulkanDevice();

		vk::ShaderModuleCreateInfo createInfo;
		createInfo.codeSize = m_ShaderInfo.Spirv.size() * sizeof(uint32_t);
		createInfo.pCode = m_ShaderInfo.Spirv.data();

		const auto [result, shaderModule] = device.createShaderModule(createInfo);

		vk::resultCheck(result, "Failed to create shader module");

		m_ShaderModule = shaderModule;
		vk::PipelineShaderStageCreateInfo shaderStageInfo;
		shaderStageInfo.module = m_ShaderModule;
		shaderStageInfo.pName = "main";

		switch (static_cast<ShaderType>(m_ShaderInfo.type))
		{
		case ShaderType::Vertex: shaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex; break;
		case ShaderType::Fragment: shaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment; break;
		case ShaderType::Compute: shaderStageInfo.stage = vk::ShaderStageFlagBits::eCompute; break;
		case ShaderType::None: __debugbreak(); break;
		}
		
		m_ShaderStateInfo = shaderStageInfo;

		const auto& ubos = m_ShaderInfo.reflectionInfo.ubos;
		const auto& ssbos = m_ShaderInfo.reflectionInfo.ssbos;
		const auto& textures = m_ShaderInfo.reflectionInfo.textures;
		const auto& images = m_ShaderInfo.reflectionInfo.images;
		const auto& pushConstants = m_ShaderInfo.reflectionInfo.pushConstants;

		for (const auto& constant : pushConstants)
		{
			const auto& constantInfo = constant.second;
			vk::PushConstantRange pushConstant{};
			pushConstant.offset = 0;
			pushConstant.stageFlags = shaderStageInfo.stage;
			pushConstant.size = 128; //IDK man

			m_PushConstantRanges.emplace_back(pushConstant);

			//pushConstant.size = constantInfo.Name;
			
		}

		for (const auto& ubo : ubos)
		{
			auto& bufferInfo = ubo.second;
			
			vk::DescriptorSetLayoutBinding binding{};
			binding.descriptorType = vk::DescriptorType::eUniformBuffer;
			binding.descriptorCount = 1;
			binding.stageFlags = m_ShaderStateInfo.stage;
			binding.binding = bufferInfo.Binding;

			vk::WriteDescriptorSet write{};
			write.descriptorCount = 1;
			write.dstArrayElement = 0;
			write.pBufferInfo = nullptr;
			write.pImageInfo = nullptr;
			write.dstBinding = bufferInfo.Binding;
			write.descriptorType = binding.descriptorType;

			m_DescWriteSet[bufferInfo.Set][binding.binding] = write;
			

			m_ShaderBindings[bufferInfo.Set].emplace_back(binding);
		}


		for (const auto& ssbo : ssbos)
		{
			auto& bufferInfo = ssbo.second;
			vk::DescriptorSetLayoutBinding binding{};
			binding.descriptorType = vk::DescriptorType::eStorageBuffer;
			binding.descriptorCount = 1;
			binding.stageFlags = m_ShaderStateInfo.stage;
			binding.binding = bufferInfo.Binding;


			vk::WriteDescriptorSet write{};
			write.descriptorCount = 1;
			write.dstArrayElement = 0;
			write.pBufferInfo = nullptr;
			write.pImageInfo = nullptr;
			write.dstBinding = bufferInfo.Binding;
			write.descriptorType = binding.descriptorType;

			m_DescWriteSet[bufferInfo.Set][binding.binding] = write;
		
			m_ShaderBindings[bufferInfo.Set].emplace_back(binding);
		}


		for (auto& texture : textures)
		{
			auto& bufferInfo = texture.second;
			vk::DescriptorSetLayoutBinding binding{};
			binding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			binding.descriptorCount = bufferInfo.arrayLength;
			binding.stageFlags = m_ShaderStateInfo.stage;
			binding.binding = bufferInfo.Binding;

			vk::WriteDescriptorSet write{};
			write.descriptorCount = bufferInfo.arrayLength;
			write.dstArrayElement = 0;
			write.pBufferInfo = nullptr;
			write.pImageInfo = nullptr;
			write.dstBinding = bufferInfo.Binding;
			write.descriptorType = binding.descriptorType;

			m_DescWriteSet[bufferInfo.Set][binding.binding] = write;

			m_ShaderBindings[bufferInfo.Set].emplace_back(binding);
		}


		for (auto& image : images)
		{
			auto& bufferInfo = image.second;
			vk::DescriptorSetLayoutBinding binding{};

			switch (bufferInfo.imageType)
			{
			case ShaderImageType::SamplerBuffer: binding.descriptorType = vk::DescriptorType::eUniformTexelBuffer; break;
			case ShaderImageType::ImageBuffer: binding.descriptorType = vk::DescriptorType::eStorageTexelBuffer; break;
			case ShaderImageType::SamplerImage: binding.descriptorType = vk::DescriptorType::eStorageImage; break;
			default: binding.descriptorType = vk::DescriptorType::eSampledImage;
			}

			
			binding.descriptorCount = bufferInfo.arrayLength;
			binding.stageFlags = m_ShaderStateInfo.stage;
			binding.binding = bufferInfo.Binding;

			vk::WriteDescriptorSet write{};
			write.descriptorCount = bufferInfo.arrayLength;
			write.dstArrayElement = 0;
			write.pBufferInfo = nullptr;
			write.pImageInfo = nullptr;
			write.dstBinding = bufferInfo.Binding;
			write.descriptorType = binding.descriptorType;

			m_DescWriteSet[bufferInfo.Set][binding.binding] = write;

			m_ShaderBindings[bufferInfo.Set].emplace_back(binding);
		}


	}


	void VulkanShader::Destroy()
	{
		auto device = m_Context->GetDevice()->GetVulkanDevice();
		device.destroyShaderModule(m_ShaderModule);

	}

	vk::PipelineShaderStageCreateInfo VulkanShader::GetVulkanPipelineStageInfo()
	{
		return m_ShaderStateInfo;
	}

	std::map<uint32_t, std::vector<vk::DescriptorSetLayoutBinding>> VulkanShader::GetVulkanDescriptorBindings()
	{
		return m_ShaderBindings;
	}

	std::vector<vk::PushConstantRange> VulkanShader::GetPushContantRange()
	{
		return m_PushConstantRanges;
	}

	DescWriteMap VulkanShader::GetDescWriteMap()
	{
		return m_DescWriteSet;
	}

	VulkanShader::~VulkanShader()
	{
	}

	void VulkanShader::Bind() const
	{
	}

	void VulkanShader::Unbind() const
	{
	}

	ReflectionInfo VulkanShader::GetShaderReflectionInfo()
	{
		return m_ShaderInfo.reflectionInfo;
	}

	ShaderType VulkanShader::GetType()
	{
		return static_cast<ShaderType>(m_ShaderInfo.type);
	}

	vk::PipelineShaderStageCreateInfo VulkanShader::GetVulkanInfo()
	{
		return m_ShaderStateInfo;
	}
}
