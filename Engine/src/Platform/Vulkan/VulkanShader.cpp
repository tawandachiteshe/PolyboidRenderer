#include "boidpch.h"
#include "VulkanShader.h"
#include <vulkan/vulkan.hpp>

#include "VkRenderAPI.h"
#include "Utils/VulkanDescriptorPool.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	VulkanShader::VulkanShader(const VkRenderAPI* context, ShaderBinaryAndInfo info): m_Context(context), m_ShaderInfo(
		std::move(info))
	{

		auto device = context->GetDevice()->GetDevice();
		auto pool = context->GetPool()->GetPool();

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


		vk::DescriptorSetLayoutCreateInfo setLayoutCreateInfo;
		vk::DescriptorSetLayoutBinding binding;
		binding.descriptorType = vk::DescriptorType::eUniformBuffer;
		binding.pImmutableSamplers = nullptr;
		binding.descriptorCount = 1;
		binding.binding = 0;
		binding.stageFlags = shaderStageInfo.stage;

		
		setLayoutCreateInfo.bindingCount = 1;
		setLayoutCreateInfo.pBindings = &binding;

		auto[layoutResult, layout] = device.createDescriptorSetLayout(setLayoutCreateInfo);

		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		auto[allocResult, descriptorSets] = device.allocateDescriptorSets(allocInfo);


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

	void VulkanShader::SetUniformBuffer(const std::string& name, const Ref<UniformBuffer>& value)
	{
	}

	void VulkanShader::SetShaderStorageBuffer(const std::string& name, const Ref<ShaderStorageBuffer>& value)
	{
	}

	void VulkanShader::SetSampler(const std::string& name, const Ref<SamplerState>& value)
	{
	}

	void VulkanShader::SetImage(const std::string& name, const Ref<SamplerState>& value)
	{
	}

	void VulkanShader::SetTexture2D(const std::string& name, const Ref<Texture>& value)
	{
	}

	void VulkanShader::SetTexture3D(const std::string& name, const Ref<Texture3D>& value)
	{
	}

	Ref<UniformBuffer> VulkanShader::GetUniformBuffer(const std::string& name) const
	{
		if (m_UniformBuffers.contains(name))
		{
			return std::reinterpret_pointer_cast<UniformBuffer>(m_UniformBuffers.at(name));
		}

		return nullptr;
	}

	Ref<ShaderStorageBuffer> VulkanShader::GetShaderStorageBuffer(const std::string& name) const
	{
		if (m_SSOBBuffers.contains(name))
		{
			return std::reinterpret_pointer_cast<ShaderStorageBuffer>(m_SSOBBuffers.at(name));
		}

		return nullptr;
	}

	Ref<SamplerState> VulkanShader::GetSampler(const std::string& name) const
	{
		if (m_Samplers.contains(name))
		{
			return std::reinterpret_pointer_cast<SamplerState>(m_Samplers.at(name));
		}

		return nullptr;
	}

	Ref<SamplerState> VulkanShader::GetImage(const std::string& name) const
	{

		if (m_Images.contains(name))
		{
			return std::reinterpret_pointer_cast<SamplerState>(m_Images.at(name));
		}

		return nullptr;
	}

	ShaderType VulkanShader::GetType()
	{
		return static_cast<ShaderType>(m_ShaderInfo.type);
	}
}
