#pragma once
#include "Engine/Engine/Shaders/ShaderCompiler.h"
#include "Engine/Renderer/Shader.h"
#include <vulkan/vulkan_raii.hpp>

namespace Polyboid
{
	class VulkanShaderStorage;
	class VulkanUniformBuffer;
	class VulkanTexture3D;
	class VulkanTexture2D;
	class VkRenderAPI;

	using DescWriteMap = std::map<uint32_t, std::map<uint32_t, vk::WriteDescriptorSet>>;



	class VulkanShader : public Shader
	{
	private:
		const  VkRenderAPI* m_Context = nullptr;
		ShaderBinaryAndReflectionInfo m_ShaderInfo;
		vk::ShaderModule m_ShaderModule;
		vk::PipelineShaderStageCreateInfo m_ShaderStateInfo;
		vk::DescriptorSetLayout m_Layout;
		std::map<uint32_t, std::vector<vk::DescriptorSetLayoutBinding>> m_ShaderBindings;
		std::vector<vk::PushConstantRange> m_PushConstantRanges;
		DescWriteMap m_DescWriteSet;
		std::unordered_map<std::string, ResourceBindingInfo> m_ResourceBindingInfo;

		//
		// std::map<std::string, Ref<VulkanUniformBuffer>> m_UniformBuffers;
		// std::map<std::string, Ref<VulkanShaderStorage>> m_SSOBBuffers;
		// std::map<std::string, Ref<SamplerState>> m_Samplers;
		// std::map<std::string, Ref<VulkanTexture2D>> m_Textures2D;
		// std::map<std::string, Ref<VulkanTexture3D>> m_Textures3D;
		// std::map<std::string, Ref<SamplerState>> m_Images;

	public:
		explicit VulkanShader(const VkRenderAPI* context, ShaderBinaryAndReflectionInfo info);

		virtual void Destroy();
		virtual vk::PipelineShaderStageCreateInfo GetVulkanPipelineStageInfo();
		virtual std::map<uint32_t, std::vector<vk::DescriptorSetLayoutBinding>> GetVulkanDescriptorBindings();
		virtual std::vector<vk::PushConstantRange> GetPushContantRange();
		virtual DescWriteMap GetDescWriteMap();
		ShaderResourceRegistry& GetShaderResourceType() override;

		~VulkanShader() override;
		void Bind() const override;
		void Unbind() const override;

		ReflectionInfo GetShaderReflectionInfo() override;

		[[nodiscard]] ShaderType GetType() override;
		

		virtual  vk::PipelineShaderStageCreateInfo GetVulkanInfo();


	};

}

