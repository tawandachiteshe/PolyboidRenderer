#pragma once
#include "Engine/Renderer/PipelineState.h"
#include <vulkan/vulkan.hpp>

#include "VkRenderAPI.h"
#include "VulkanDepthStencilState.h"
#include "VulkanBlendState.h"
#include "VulkanRasterizerState.h"

namespace Polyboid
{
	class VulkanGraphicsPipeline : public PipelineState
	{

	private:
		ShaderMap m_Shaders;
		const VkRenderAPI* m_Context = nullptr;
		bool m_Dirty = true;
		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_PipelineLayout;

		VulkanDepthStencilState m_DepthStencil;
		VulkanBlendState m_BlendState;
		VulkanRasterizerState m_RasterizerState;
		Ref<VertexBufferArray> m_VertexInput;

	public:
		explicit VulkanGraphicsPipeline(const VkRenderAPI* context);


		void SetShader(const ShaderType& type, const Ref<Shader>& shader) override;
		void SetShader(const Ref<Shader>& shader) override;
		std::shared_ptr<Shader> GetShader(const ShaderType& type) const override;
		[[nodiscard]] const ShaderMap& GetShaders() const override;
		void SetBlendState(const BlendState& blendState) override;
		BlendState& GetBlendState() override;
		void SetRasterizerState(const RasterizerState& rasterizerState) override;
		RasterizerState& GetRasterizerState() override;
		void SetDepthStencilState(const DepthStencilState& depthStencilState) override;
		DepthStencilState& GetDepthStencilState() override;
		void SetVertexArray(const Ref<VertexBufferArray>& va) override;
		 Ref<VertexBufferArray>& GetVertexArray() override;
		~VulkanGraphicsPipeline() override;
		void Bind() override;
		void UnBind() override;
	};

}

