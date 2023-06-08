#pragma once
#include "Engine/Renderer/GraphicsPipeline.h"
#include <vulkan/vulkan_raii.hpp>

#include "VkRenderAPI.h"
#include "VulkanDepthStencilState.h"
#include "VulkanBlendState.h"
#include "VulkanMultiSampleState.h"
#include "VulkanRasterizerState.h"
#include "VulkanVertexBufferArray.h"

namespace Polyboid
{
	class VulkanGraphicsPipeline : public GraphicsPipeline
	{

	private:


		ShaderMap m_Shaders;
		const VkRenderAPI* m_Context = nullptr;
		bool m_Dirty = true;
		std::vector<vk::PipelineShaderStageCreateInfo> m_Stages;

		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_PipelineLayout;
		std::vector<vk::DescriptorSetLayout> m_DescriptorSetLayouts;
		std::unordered_map < uint32_t, std::vector<Ref<PipelineDescriptorSet>>> m_Sets;
		DescWriteMap m_DescWriteMap;
		vk::GraphicsPipelineCreateInfo m_CreatePipelineInfo{};

		VulkanDepthStencilState m_DepthStencil;
		VulkanBlendState m_BlendState;
		VulkanRasterizerState m_RasterizerState;
		VulkanMultiSampleState m_MultiSampleState;
		Ref<VulkanVertexBufferArray> m_VertexInput;
		Ref<VulkanRenderPass> m_RenderPass = nullptr;

		vk::PrimitiveTopology m_Topology = vk::PrimitiveTopology::eTriangleList;

		std::map<uint32_t, std::vector<vk::DescriptorSetLayoutBinding>> m_Bindings;
		std::map<uint32_t, vk::DescriptorSetLayout> m_SetIndexWithLayout;
		std::vector<vk::PushConstantRange> m_PushConstantRanges;
		Ref<VulkanPipelineDescriptorSetPool> m_DescPool;
		std::vector<Ref<PipelineDescriptorSet>> m_PipeDescSets;

		std::unordered_map<uint32_t, std::unordered_map<uint32_t, Ref<UniformBufferSet>>> m_UniformBufferSets;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, Ref<StorageBufferSet>>> m_StorageBufferSets;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, Ref<Texture>>> m_TextureSets;


	public:
		explicit VulkanGraphicsPipeline(const VkRenderAPI* context);

		void Bake() override;
		void Init();
		void Recreate() override;

		void SetGeometryTopology(const PrimitiveType& primitiveType) override;
		void SetShader(const ShaderType& type, const Ref<Shader>& shader) override;
		void SetShader(const Ref<Shader>& shader) override;

		Ref<Shader> GetShader(const ShaderType& type) const override;
		[[nodiscard]] const ShaderMap& GetShaders() const override;

		void SetBlendState(const BlendState& blendState) override;
		BlendState& GetBlendState() override;

		void SetRasterizerState(const RasterizerState& rasterizerState) override;
		RasterizerState& GetRasterizerState() override;

		void SetRenderPass(const Ref<RenderPass>& renderPass) override;
		Ref<RenderPass> GetRenderPass() override;

		void SetDepthStencilState(const DepthStencilState& depthStencilState) override;
		DepthStencilState& GetDepthStencilState() override;

		void SetVertexArray(const Ref<VertexBufferArray>& va) override;
		 Ref<VertexBufferArray> GetVertexArray() override;

		void SetMultiSampleState(const MultiSampleState& multiSampleState) override;
		MultiSampleState& GetMutltiSampleState() override;

		void SetGraphicsShaders(const GraphicsShaders& shaders) override;

		virtual vk::PipelineLayout GetPipelineLayout();

		~VulkanGraphicsPipeline() override;
		void Bind() override;
		void UnBind() override;

		std::vector<Ref<PipelineDescriptorSet>> AllocateDescriptorSets(uint32_t setBinding) override;

		std::vector<Ref<PipelineDescriptorSet>> GetDescriptorSets(uint32_t set) override;

		void BindUniformBufferSet(uint32_t binding, const Ref<UniformBufferSet>& bufferSet,
			uint32_t setBinding) override;
		void BindStorageBufferSet(uint32_t binding, const Ref<StorageBufferSet>& bufferSet,
			uint32_t setBinding) override;
		void BindTexture2D(uint32_t binding, const Ref<Texture>& bufferSet, uint32_t setBinding) override;

		void WriteSetResourceBindings(uint32_t set) override;

		void Destroy() override;



		std::any GetHandle() override;
	};

	
}

