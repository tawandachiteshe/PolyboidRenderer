#pragma once
#include "Engine/Engine/Shaders/ShaderCompiler.h"
#include "Engine/Renderer/Shader.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VulkanShaderStorageBuffer;
	class VulkanUniformBuffer;
	class VulkanTexture3D;
	class VulkanTexture2D;
	class VkRenderAPI;

	class VulkanShader : public Shader
	{
	private:
		const  VkRenderAPI* m_Context = nullptr;
		ShaderBinaryAndInfo m_ShaderInfo;
		vk::ShaderModule m_ShaderModule;
		vk::PipelineShaderStageCreateInfo m_ShaderStateInfo;
		vk::DescriptorSetLayout m_Layout;

		std::map<std::string, Ref<VulkanUniformBuffer>> m_UniformBuffers;
		std::map<std::string, Ref<VulkanShaderStorageBuffer>> m_SSOBBuffers;
		std::map<std::string, Ref<SamplerState>> m_Samplers;
		std::map<std::string, Ref<VulkanTexture2D>> m_Textures2D;
		std::map<std::string, Ref<VulkanTexture3D>> m_Textures3D;
		std::map<std::string, Ref<SamplerState>> m_Images;

	public:
		explicit VulkanShader(const VkRenderAPI* context, ShaderBinaryAndInfo info);


		~VulkanShader() override;
		void Bind() const override;
		void Unbind() const override;
		void SetUniformBuffer(const std::string& name, const Ref<UniformBuffer>& value) override;
		void SetShaderStorageBuffer(const std::string& name, const Ref<ShaderStorageBuffer>& value) override;
		void SetSampler(const std::string& name, const Ref<SamplerState>& value) override;
		void SetImage(const std::string& name, const Ref<SamplerState>& value) override;
		void SetTexture2D(const std::string& name, const Ref<Texture>& value) override;
		void SetTexture3D(const std::string& name, const Ref<Texture3D>& value) override;
		[[nodiscard]] Ref<UniformBuffer> GetUniformBuffer(const std::string& name) const override;
		[[nodiscard]] Ref<ShaderStorageBuffer> GetShaderStorageBuffer(const std::string& name) const override;
		[[nodiscard]] Ref<SamplerState> GetSampler(const std::string& name) const override;
		[[nodiscard]] Ref<SamplerState> GetImage(const std::string& name) const override;
		[[nodiscard]] ShaderType GetType() override;
	};

}

